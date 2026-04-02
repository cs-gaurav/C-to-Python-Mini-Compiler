const textarea = document.querySelector('.editor');

textarea.addEventListener('keydown', function(e) {
    if (e.key === 'Tab') {
        e.preventDefault();
        const start = this.selectionStart;
        const end = this.selectionEnd;

        this.value = this.value.substring(0, start) + "\t" + this.value.substring(end);
        this.selectionStart = this.selectionEnd = start + 1;
    }
});

function compileCode() {
    const code = document.querySelector('.editor').value;

    
    const form = document.createElement("form");
    form.method = "POST";
    form.action = "/compile";

    const input = document.createElement("input");
    input.type = "hidden";
    input.name = "code";
    input.value = code;

    form.appendChild(input);
    document.body.appendChild(form);

    
    form.submit();
}
