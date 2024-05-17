<script>
    import { onMount } from 'svelte';
    import { auth } from './javascript/firebase'; // Import Firebase auth instance
    
    let currentUser;
  
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
  
  <h1>Welcome to your profile page, {currentUser && currentUser.displayName}!</h1>
  
  {#if currentUser}
    <p>Email: {currentUser.email}</p>
    <p>Profile Picture: <img src={currentUser.photoURL} alt="Profile Picture" width="100"></p>
    <button on:click={logout}>Logout</button>
  {:else}
    <p>Loading...</p>
  {/if}
  