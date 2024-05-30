<script>
    import { onMount } from "svelte";
    import { user } from "../javascript/authstore"; // Import the user store
    import { get } from "svelte/store"; // To access the store value
    import { db } from "../javascript/firebase";
    import { collection, getDocs, query, where, setDoc, doc } from "firebase/firestore";
    import { logout } from "../javascript/auth.js"; // Import the logout function
    import { goto } from "$app/navigation"; // Import SvelteKit's goto function for redirection

    let currentUser = get(user);
    let isAdmin = [];

    // Watch for changes in the user store


    onMount(async () => {
        
        if (currentUser?.uid) {
            const q = query(
                collection(db, "admins"),
                where("userId", "==", currentUser?.uid),
            );
            const querySnapshot = await getDocs(q);
            isAdmin = querySnapshot.docs.map((doc) => ({
                id: doc.id,
                ...doc.data(),
            }));
            console.log(isAdmin);
        }
    });

</script>


<nav id="navbar" class="navbar">
    <a href="/">Home</a>
    <a href="/profile">Profile</a>
    <a href="/list">Courses</a>
    {#if isAdmin.length > 0}
    <a href="/create">Admin</a>
    {/if}
</nav>

<style>
    #navbar {
        background-color: #333;
        color: #fff;
        padding: 10px 0;
        margin-left: 0px;
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
