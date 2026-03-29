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

    fetch('/compile', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: 'code=' + encodeURIComponent(code)
    })
    .then(response => response.text())
    .then(data => {
        document.open();
        document.write(data);
        document.close();
    })
    .catch(error => console.error(error));
}