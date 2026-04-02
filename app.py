from flask import Flask, render_template, request
import subprocess

app = Flask(__name__)

@app.route("/")
def editor():
    return render_template("editor.html")


@app.route('/compile', methods=['POST'])
def compile_code():
    code = request.form.get('code')

    try:
        result = subprocess.run(
            ["main.exe"],  
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

        section = "lexer"

        syntax_error = False
        semantic_error = False

        for line in lines:
            line_lower = line.lower()

            
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

            
            if "undeclared" in line_lower or "semantic error" in line_lower:
                semantic = [line.strip()]
                semantic_error = True
                section = "semantic"
                continue

            
            if "parsing complete" in line_lower:
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

       

        
        if syntax_error:
            semantic = []
            tac = []
            python_code = []

        else:
            parser = ["No Syntax Error"]

            
            if semantic_error:
                tac = []
                python_code = []

            
            else:
                semantic = ["No Semantic Error"]

        return render_template(
            "phase.html",
            lexer="\n".join(lexer),
            parser="\n".join(parser),
            semantic="\n".join(semantic),
            tac="\n".join(tac),
            python_code="\n".join(python_code)
        )

    except Exception as e:
        return f"<pre>{str(e)}</pre>"


if __name__ == '__main__':
    app.run(debug=True)
