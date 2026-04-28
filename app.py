from flask import Flask, render_template, request
import subprocess
import sys
import os
from graphviz import Source

app = Flask(__name__)

def get_executable():
    if sys.platform == "win32":
        exe = "main.exe"
    else:
        exe = "./main"
    
    if not os.path.exists(exe.lstrip("./")):
        script_dir = os.path.dirname(os.path.abspath(__file__))
        exe = os.path.join(script_dir, "main.exe" if sys.platform == "win32" else "main")
    
    return exe


@app.route("/")
def editor():
    return render_template("editor.html")


@app.route('/compile', methods=['POST'])
def compile_code():
    code = request.form.get('code')

    try:
        executable = get_executable()

        result = subprocess.run(
            [executable],
            input=code + "\n\x1a",
            capture_output=True,
            text=True
        )

        full_output = result.stdout + result.stderr
        lines = full_output.splitlines()

        lexer = []
        parser = []
        semantic = []
        tac = []
        python_code = []

        dot_code = ""
        capture_tree = False

        section = "lexer"
        syntax_error = False
        semantic_error = False

        for line in lines:
            line_lower = line.lower()

            # -------- PARSE TREE --------
            if "parse_tree_start" in line_lower:
                capture_tree = True
                continue

            if "parse_tree_end" in line_lower:
                capture_tree = False
                continue

            if capture_tree:
                dot_code += line + "\n"
                continue
            # ----------------------------

            if "compiler has started" in line_lower:
                continue

            if "give some expression" in line_lower:
                continue

            if line.strip() == "":
                continue

            if "syntax error" in line_lower or "invalid" in line_lower:
                parser = [line.strip()]
                syntax_error = True
                section = "parser"
                continue

            if "unexpected token" in line_lower:
                parser.append(line.strip())
                continue

            if "undeclared" in line_lower or "semantic error" in line_lower:
                semantic = [line.strip()]
                semantic_error = True
                section = "semantic"
                continue

            if "parsing successful" in line_lower:
                section = "parser"
                continue

            elif "semantic analysis done" in line_lower:
                section = "tac"
                continue

            elif "intermediate code generated" in line_lower:
                section = "tac"
                continue

            elif "generated python code" in line_lower:
                section = "python"
                continue

            if section == "lexer":
                lexer.append(line)

            elif section == "tac":
                if "----" not in line:
                    tac.append(line)

            elif section == "python":
                if "----" not in line:
                    python_code.append(line)

        # -------- GRAPHVIZ IMAGE --------
        image_path = None
        if dot_code.strip() and not syntax_error and not semantic_error:
            try:
                graph = Source(dot_code)
                image_path = "static/parse_tree.png"
                graph.render("static/parse_tree", format="png", cleanup=True)
            except Exception as e:
                print("Graphviz error:", e)
        # --------------------------------

        if not syntax_error:
            parser = ["Parsing Successful"]

        if not semantic_error:
            semantic = ["No Semantic Error"]

        return render_template(
            "phase.html",
            lexer="\n".join(lexer),
            parser="\n".join(parser),
            semantic="\n".join(semantic),
            tac="\n".join(tac),
            python_code="\n".join(python_code),
            tree_image=image_path,
            syntax_error=syntax_error,
            semantic_error=semantic_error
        )

    except Exception as e:
        return f"<pre>{str(e)}</pre>"


if __name__ == '__main__':
    app.run(debug=True)