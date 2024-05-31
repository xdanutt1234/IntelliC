<script lang="ts">
    import { onMount } from "svelte";
    import { user } from "../../javascript/authstore.js"; // Import the user store
    import { get } from "svelte/store"; // To access the store value
    import { logout } from "../../javascript/auth.js"; // Import the logout function
    import { goto } from "$app/navigation"; // Import SvelteKit's goto function for redirection
    import Navbar from "../../resources/Navbar.svelte";
    import { collection, getDocs, query, where } from "firebase/firestore";
    import { db } from "../../javascript/firebase"; // Import the Firestore instance from your firebase.js

    let currentUser = get(user);
    let solvedCount = 0;

    // Watch for changes in the user store
    $: currentUser = $user;

    // Function to handle logout
    const handleLogout = async () => {
        try {
            await logout();
            console.log("User signed out successfully.");
            goto("/login"); // Redirect to the login page after logout
        } catch (error) {
            console.error("An error occurred while signing out:", error);
        }
    };

    // Function to count solved problems
    async function countSolvedProblems() {
        try {
            const q = query(collection(db, "exercises"), where("solved", "==", true));
            const querySnapshot = await getDocs(q);
            solvedCount = querySnapshot.size;
        } catch (error) {
            console.error("Error counting solved problems:", error);
        }
    }

    onMount(() => {
        countSolvedProblems();
    });
</script>

<style>
    body, html {
        margin: 0;
        padding: 0;
    }
    .profile-container {
        max-width: 800px;
        margin: 2rem auto;
        padding: 2rem;
        margin-top: 200px;
        background-color: #f9f9f9;
        border-radius: 10px;
        box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
    }
    .profile-header {
        display: flex;
        align-items: center;
        gap: 1rem;
    }
    .profile-header img {
        border-radius: 50%;
    }
    .profile-header h1 {
        margin: 0;
    }
    .profile-details {
        margin-top: 1rem;
    }
    .profile-details p {
        margin: 0.5rem 0;
    }
    .logout-button {
        margin-top: 1rem;
        padding: 0.5rem 1rem;
        background-color: #ff6b6b;
        border: none;
        border-radius: 5px;
        color: white;
        cursor: pointer;
        transition: background-color 0.3s;
    }
    .logout-button:hover {
        background-color: #ff4c4c;
    }
    .solved-counter {
        margin-top: 1rem;
        padding: 0.5rem 1rem;
        background-color: #4caf50;
        color: white;
        border-radius: 5px;
        text-align: center;
    }
</style>

<Navbar />

<div class="profile-container">
    {#if currentUser}
        <div class="profile-header">
            <img src={currentUser.photoURL} alt="Profile Picture" width="100">
            <h1>Welcome, {currentUser.displayName}!</h1>
        </div>
        <div class="profile-details">
            <p><strong>Email:</strong> {currentUser.email}</p>
        </div>
        <div class="solved-counter">
            <p><strong>Solved Problems:</strong> {solvedCount}</p>
        </div>
        <button class="logout-button" on:click={handleLogout}>Logout</button>
    {:else}
        <p>Loading...</p>
    {/if}
</div>
