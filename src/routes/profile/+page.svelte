<script lang="ts">
    import { user } from "../../javascript/authstore.js"; // Import the user store
    import { get } from "svelte/store"; // To access the store value
    import { logout } from "../../javascript/auth.js"; // Import the logout function
    import { goto } from "$app/navigation"; // Import SvelteKit's goto function for redirection

    import Navbar from "../../resources/Navbar.svelte";

    let currentUser = get(user);

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
</script>

<Navbar />

<h1>Welcome to your profile page, {currentUser && currentUser.displayName}!</h1>

{#if currentUser}
  <p>Email: {currentUser.email}</p>
  <p>Profile Picture: <img src={currentUser.photoURL} alt="Profile Picture" width="100"></p>
  <p></p>
  <button on:click={handleLogout}>Logout</button>
{:else}
    <p>Loading...</p>
{/if}
