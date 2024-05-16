<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Coding Site</title>
    <style>
        /* Basic CSS for navbar */
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
        }
        
        nav {
            background-color: #333;
            color: #fff;
            padding: 10px;
            text-align: center;
            position: fixed;
            top: -50px; /* Initially hides the navbar */
            width: 100%;
            transition: top 0.3s;
            z-index: 1000; /* Ensures the navbar stays on top of other elements */
        }
        
        nav a {
            color: #fff;
            text-decoration: none;
            padding: 10px 20px;
            margin: 0 5px;
            border-radius: 5px;
        }
        
        nav a:hover {
            background-color: #555;
        }
    </style>
</head>
<body>

<nav id="navbar">
    <a href="#">Home</a>
    <a href="#">Courses</a>
    <a href="#">Performance Stats</a>
    <a href="#">Profile</a>
    <!-- Add more links/buttons as needed -->
</nav>

<!-- Your site content goes here -->

<script>
    // JavaScript to show navbar when cursor approaches top of page
    window.addEventListener('scroll', function() {
        const navbar = document.getElementById('navbar');
        if (window.scrollY < 50) { // Change this value to adjust when the navbar appears
            navbar.style.top = '-50px'; // Hides the navbar
        } else {
            navbar.style.top = '0'; // Shows the navbar
        }
    });
</script>

</body>
</html>
