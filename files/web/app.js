
const input1 = document.getElementById("input1");
const input2 = document.getElementById("input2");
const resultElement = document.getElementById("result");

let flag = '+';

function changeflag(button) { flag = button; }
    
function calculate(inp1, inp2) {
    if (flag == '+') { return Number(inp1.value) + Number(inp2.value); } 
    if (flag == '-') { return Number(inp1.value) - Number(inp2.value); } 
    if (flag == '*') { return Number(inp1.value) * Number(inp2.value); } 
    if (flag == '/') { return Number(inp1.value) / Number(inp2.value); } 
}


if (document.getElementById('submit')) {
    document.getElementById('submit').onclick = function () {
        resultElement.textContent = calculate(input1, input2);
        Number(resultElement.textContent) < 0 ? resultElement.style.color = "red" : resultElement.style.color = "green";
    }
}


if (document.getElementById('login')) {
    document.getElementById('login').onclick = function () {
        // Log the username and password
        const username = document.getElementById('username').value;
        const password = document.getElementById('password').value;

        // Define the URL and data
        const data = {
            Username: username,
            Password: password,
            "hello": "11"
        };

        // Make the fetch request
        fetch("http://localhost:8080", {
            method: 'POST',
            mode: "cors",
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data)
        })
        .then(response => {
            console.log(response)
            if (response.ok) {
                console.error(response.url)
                window.location.href = response.url; // Замените на нужный URL
            } else {
                console.error('Ошибка при запросе:', response.status);
            }
        })
        .catch(error => console.error('Ошибка сети:', error));
    };
}

console.log("started");