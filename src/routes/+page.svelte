
<script>
    import { user } from '../javascript/authstore.js';
    import { onMount } from 'svelte';
    import { goto } from '$app/navigation';
    import Navbar from "../resources/Navbar.svelte";
  
    let userData;
  
    $: if (!$user) {
      //goto('/login');
    }
  
    $: userData = $user;
  
    onMount(() => {
      if (!$user) {
        goto('/login');
      }
    });
  </script>
<Navbar />

<h1>Welcome to SvelteKit</h1>
<p>Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation</p>
