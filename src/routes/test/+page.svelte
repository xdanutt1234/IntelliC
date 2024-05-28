<script lang="ts">
    import { user } from '../../javascript/authstore.js';
    import { onMount } from 'svelte';
    import { goto } from '$app/navigation';
  
    let userData;
  
    $: if (!$user) {
      goto('/login');
    }
  
    $: userData = $user;
  
    onMount(() => {
      if (!$user) {
        goto('/login');
      }
    });
  </script>
  
  {#if userData}
    <h1>Welcome, {userData.email}!</h1>
    <!-- Display user profile info -->
  {/if}
