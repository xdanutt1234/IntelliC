<script lang="ts">
    import { Router, Route } from "svelte-routing";
    import App from "./App.svelte";

    import { navigate } from "svelte-routing";

    const handleStart = () => {
        navigate("/app");
    };
</script>

<div class="home-container">
    <h1>IntelliC</h1>
    <button on:click={handleStart}>Start</button>
</div>

<Router>
  <Route path="/app" component={App} />
</Router>

<style>
    .home-container {
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
        height: 100vh;
        background-color: var(--primary-bg-color);
        color: var(--primary-text-color);
    }

    h1 {
        font-size: 3rem;
        margin-bottom: 2rem;
    }

    button {
        padding: 10px 20px;
        border: none;
        background-color: var(--button-bg-color);
        color: var(--button-text-color);
        cursor: pointer;
        border-radius: 5px;
        font-size: 1rem;
    }

    button:hover {
        background-color: darken(var(--button-bg-color), 10%);
    }
</style>
