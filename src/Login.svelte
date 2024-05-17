<script>
  import { onMount } from 'svelte';
  import { getAuth, signInWithRedirect, onAuthStateChanged, GoogleAuthProvider } from 'firebase/auth';
  import { getFirestore, doc, setDoc } from 'firebase/firestore';
  import app from './javascript/firebase';
  import HomePage from './App.svelte';
  import { navigate } from 'svelte-routing';
  import { Router, Route } from 'svelte-routing';

  import Next from './App.svelte'

  const routes = [
    { path: '/next', component: Next}
  ]

  const auth = getAuth(app);
  const db = getFirestore(app);
  let redirect = false;
  // Function to handle user data storage in Firestore
  async function storeUserData(user) {
    try {
      await setDoc(doc(db, "users", user.uid), {
        email: user.email,
        name: user.displayName,
        profilePicture: user.photoURL,
        createdAt: new Date()
      });
      console.log('User data added to Firestore');
    } catch (error) {
      console.error('Error storing user data:', error.message);
    }
  }

  // Function to handle Google sign-in
  function signInWithGoogle() {
    const provider = new GoogleAuthProvider();
    provider.setCustomParameters({ prompt: 'select_account' }); // Force account chooser
    signInWithRedirect(auth, provider);
  }

  // Monitor authentication state changes
  onMount(() => {
    onAuthStateChanged(auth, (user) => {
      if (user) {
        // User is signed in
        storeUserData(user); // Store user data in Firestore
        navigate('/next', { replace: true });
        // Redirect or navigate to the next page
      } else {
        // User is signed out
      }
    });
  });
</script>

<Router>
  {#each routes as { path, component }}
    <Route {path} let:params>
      <svelte:component this={component} {...params} />
    </Route>
  {/each}
</Router>


  <div class="container" style="margin-top: 300px;">
    <h1>Welcome to Our App</h1>
    <p>Please sign in to continue</p>
    <button class="google-btn" on:click={signInWithGoogle}>
      <img src="google-logo.png" alt="Google Logo">
      Sign in with Google
    </button>
  </div>



<style>
  /* Container styles */
  .container {
    max-width: 400px;
    margin: 0 auto;
    text-align: center;
    padding: 40px;
    background-color: #f9f9f9;
    border-radius: 8px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
  }

  /* Heading styles */
  h1 {
    font-size: 24px;
    margin-bottom: 20px;
  }

  /* Button styles */
  .google-btn {
    display: inline-flex;
    align-items: center;
    justify-content: center;
    padding: 12px 24px;
    border: none;
    border-radius: 6px;
    background-color: #4285f4;
    color: #fff;
    font-size: 16px;
    font-weight: bold;
    cursor: pointer;
    transition: background-color 0.3s;
  }

  .google-btn img {
    width: 24px;
    height: 24px;
    margin-right: 12px;
  }

  .google-btn:hover {
    background-color: #3c78dc;
  }
</style>
