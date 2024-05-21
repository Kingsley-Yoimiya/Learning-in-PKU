function appendExp(char) {
    document.getElementById("expression").value += char;
}

function clearAll() {
    document.getElementById("expression").value = '';
}

function calc() {
    let exp = document.getElementById("expression");
    try {
        exp.value = eval(exp.value);
    } catch {
        exp.value = "Error";
    }
}

document.getElementById("expression").addEventListener("keydown", function(event) {
    if (event.key === "Enter") {
        event.preventDefault();
        calc();
    }
});