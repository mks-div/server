// main
const input1 = document.getElementById("input1");
const input2 = document.getElementById("input2");
const resultElement = document.getElementById("result");
// buttons
const subbtn = document.getElementById('submit');

let flag = '+';


console.log("started");

function changeflag(button) { flag = button; }
    
function calculate(inp1, inp2) {
    if (flag == '+') { return Number(inp1.value) + Number(inp2.value); } 
    if (flag == '-') { return Number(inp1.value) - Number(inp2.value); } 
    if (flag == '*') { return Number(inp1.value) * Number(inp2.value); } 
    if (flag == '/') { return Number(inp1.value) / Number(inp2.value); } 
}

subbtn.onclick = function () {
    resultElement.textContent = calculate(input1, input2);

    Number(resultElement.textContent) < 0 ? resultElement.style.color = "red" : resultElement.style.color = "green";

}