


document.getElementById('login').onclick = function () {
    // Log the username and password
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    // Define the URL and data
    const data = {
        Username: username,
        Password: password
    };

    // Make the fetch request
    fetch("http://localhost:8080", {
        method: 'POST',
        mode: "cors",
        redirect: "manual",
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data)
    })
    .then(response => {
        console.log(response);
        window.location.href = "http://localhost:8080/index.html"
    })
};