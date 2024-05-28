<script lang="ts">
    import { onMount } from "svelte";
    import { auth } from "../../javascript/firebase"; // Import Firebase auth instance
    import Navbarfrom from "../../resources/Navbar.svelte";
    import Navbar from "../../resources/Navbar.svelte";

    let currentUser: any;

    // Fetch current user data when the component is mounted
    onMount(async () => {
        currentUser = auth.currentUser;
    });

    // Function to handle logout
    const logout = async () => {
        try {
            await auth.signOut();
            console.log("User signed out successfully.");
            // Redirect or perform any other action after logout.
        } catch (error) {
            console.error("An error occurred while signing out:", error);
        }
    };
</script>

<Navbar/>

<h1>Welcome to your profile page, {currentUser && currentUser.displayName}!</h1>

{#if currentUser}
    <p>Email: {currentUser.email}</p>
    <p>
        Profile Picture: <enhanced:img
            src={currentUser.photoURL}
            alt="Profile Picture"
        ></enhanced:img>
    </p>
    <button on:click={logout}>Logout</button>
{:else}
    <p>Loading...</p>
{/if}
