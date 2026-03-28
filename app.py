from flask import Flask, render_template 

app = Flask(__name__)

@app.route("/")
def editor():
    return render_template('editor.html')

@app.route("/output")
def phase():
    return render_template('phase.html')

if __name__ == '__main__':  
   app.run(debug=True)