
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

if (document.getElementById('logname')) {
    document.getElementById('logname').onclick = function () {
        document.getElementById("wrongdata").style.display = "none";
    }
}
if (document.getElementById('logpwrd')) {
    document.getElementById('logpwrd').onclick = function () {
        document.getElementById("wrongdata").style.display = "none";
    }
}
function LogToServ(username, password, token="", isreg='') {

    const data = {
            Username: username,
            Password: password,
            ISREG: isreg,
            ISAUT: (isreg ? "" : "1"),
            ISEXPR: "",
        };

    fetch("http://localhost:8080", {
        method: 'POST',
        mode: "cors",
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data)
    })
    .then(response => {
        if (response.status === 200) {
            return response.json().then(data => {
                if (data.token) {
                    sessionStorage.setItem('token', data.token);
                }
                if (data.redirect) {
                    window.location.href = data.redirect;
                }
                else {
                    throw new Error('Токен не найден в ответе');
                }
            });
        } else if (!response.ok) {
            console.log("sfddfd")
            if (document.getElementById("wrongdata")) {
                document.getElementById("wrongdata").style.display = "block";
            }
            
            throw new Error(`Ошибка HTTP: ${response.status}`);
        }
        return response.json();
    })

}

function RegToServ(username, password) {LogToServ(username, password, "", "1")}
    



function openModal(type) {
    const modal = document.getElementById('authModal');
    const loginForm = document.getElementById('loginForm');
    const registerForm = document.getElementById('registerForm');
    const modalTitle = document.getElementById('modalTitle');

    if (type === 'login') {
      modalTitle.textContent = 'Вход';
      loginForm.style.display = 'block';
      registerForm.style.display = 'none';
    } else {
      modalTitle.textContent = 'Регистрация';
      loginForm.style.display = 'none';
      registerForm.style.display = 'block';
    }

    modal.style.display = 'flex';
}

function openOut() {
    console.log("openede")
    const modal = document.getElementById('outModal');
    const loginForm = document.getElementById('outForm');
    const modalTitle = document.getElementById('modalTitle');

    //modalTitle.textContent = 'Вход';
    outForm.style.display = 'block';
    //registerForm.style.display = 'none';
    modal.style.display = 'flex';
}

function closeModal() {
    document.getElementById('authModal').style.display = 'none';
}

function closeOut() {
    document.getElementById('outModal').style.display = 'none';
}

// Закрытие модального окна при клике вне его
window.onclick = function(event) {
    const modal = document.getElementById('authModal');
    const out = document.getElementById('outModal');
    if (event.target == modal) {
        closeModal();
    }

    if (event.target == out) {
        closeOut();
    }
}





if (document.getElementById('login')) {
    document.getElementById('login').onclick = function () {
        const username = document.getElementById('username').value
        const password = document.getElementById('password').value
        LogToServ(username, password)
    };
}
if (document.getElementById("expression")) {
    document.getElementById("submit").onclick = 
    function PasteExpr() {
        console.log(document.getElementById("expression").value)
        const data = {
                Token: sessionStorage.getItem("token"),
                Expression: document.getElementById("expression").value,
            };

        fetch("http://localhost:8080", {
            method: 'POST',
            mode: "cors",
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data)
        })
        .then(response => {
            if (response.status === 200) {
                return response.json().then(data => {
                    console.log(data.answer);
                    document.getElementById("result").innerText = data.answer;
                });
            } else if (!response.ok) {
                throw new Error(`Ошибка HTTP: ${response.status}`);
            }
            return response.json();
        })

    }
}


if (document.getElementById("authModal")) {
    document.getElementById('authModalLog').onclick = function () {
        const username = document.getElementById('logname').value
        const password = document.getElementById('logpwrd').value
        LogToServ(username, password)
    };

    document.getElementById('authModalReg').onclick = function () {
        const username = document.getElementById('regname').value
        const password = document.getElementById('regpwrd').value
        RegToServ(username, password)
    };
}
if (document.getElementById("outModal")) {
    document.getElementById('outModalLog').onclick = function () {
        sessionStorage.removeItem("token");
        window.location.href = "http://localhost:8080";
    };
}
console.log("started");