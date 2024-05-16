<script>
    import { onMount } from 'svelte';
    import { getAuth, signInWithRedirect, onAuthStateChanged } from 'firebase/auth';
    import { GoogleAuthProvider } from 'firebase/auth'; // Import GoogleAuthProvider
    import app from './javascript/firebase';
  
    const auth = getAuth(app);
  
    onMount(() => {
      onAuthStateChanged(auth, (user) => {
        if (user) {
          // User is signed in, redirect or navigate to the next page
        } else {
          // User is signed out
        }
      });
    });
  
    function signInWithGoogle() {
      signInWithRedirect(auth, new GoogleAuthProvider());
    }
  </script>
  
  <button on:click={signInWithGoogle}>Sign in with Google</button>
  
  