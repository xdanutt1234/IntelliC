<nav id="navbar" class="navbar">
    <a href="/" >Home</a>
    <a href="/code">Code</a>
    <a href="/profile">Profile</a>
    <a href="/login"> Log Out</a>
</nav>
<style>
    #navbar {
        background-color: #333;
        color: #fff;
        padding: 10px;
        text-align: center;
        position: fixed;
        top: 0; /* Ensures the navbar is always at the top */
        width: 100%;
        z-index: 1000; /* Ensures the navbar stays on top of other elements */
    }

    #navbar a {
        color: #fff;
        text-decoration: none;
        padding: 10px 20px;
        margin: 0 5px;
        border-radius: 5px;
    }

    #navbar a:hover {
        background-color: #555;
    }
</style>
