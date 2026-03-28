from flask import Flask, render_template, request
import subprocess

app = Flask(__name__)

@app.route('/')
def editor():
    return render_template('editor.html')


@app.route('/compile', methods=['POST'])
def compile_code():
    code = request.form.get('code') or request.data.decode()

    with open("input.c", "w") as f:
        f.write(code)

    try:
        result = subprocess.run(
            ["./main.exe"],
            input=code,
            capture_output=True,
            text=True
        )

        full_output = result.stdout + result.stderr

       
        lexer = []
        parser = []
        semantic = []
        intermediate = []
        python_code = []

        syntax_error = False

        
        raw_lines = full_output.splitlines()
        lines = []

        for line in raw_lines:
            line_lower = line.lower()

            if "compiler has started" in line_lower:
                continue
            if "give some expression" in line_lower:
                continue
            if line.strip() == "":
                continue

            lines.append(line)

        section = "lexer"

        for line in lines:
            line_lower = line.lower()

            
            if "error" in line_lower:
                parser.append(line)
                syntax_error = True
                section = "parser"
                continue

           
            if "parsing complete" in line_lower:
                section = "parser"
                continue

            elif "semantic analysis done" in line_lower:
                section = "semantic"
                continue

            elif "intermediate code generated" in line_lower:
                section = "intermediate"
                continue

            elif "generated python code" in line_lower:
                section = "python"
                continue

            
            if section == "lexer":
                lexer.append(line)

            elif section == "parser":
                parser.append(line)

            elif section == "semantic":
                intermediate.append(line)

            elif section == "python":
                python_code.append(line)

       

        if not syntax_error:
            parser = ["No Syntax Error"]
            semantic = ["No Semantic Error"]
        else:
            
            semantic = []
            intermediate = []
            python_code = []

        return render_template(
            "phase.html",
            lexer="\n".join(lexer).strip(),
            parser="\n".join(parser).strip(),
            semantic="\n".join(semantic).strip(),
            intermediate="\n".join(intermediate).strip(),
            python_code="\n".join(python_code).strip()
        )

    except Exception as e:
        return f"<pre>{str(e)}</pre>"


if __name__ == '__main__':
    app.run(debug=True)
