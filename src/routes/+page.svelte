<script lang="ts">
    import { goto } from "$app/navigation";

   function goToLogin(){
        goto("/login");
   } 
</script>

<div class="container">
    <h1>IntelliC</h1>
    <button on:click={goToLogin}>Click Me</button>
</div>

<style>
    .container {
        text-align: center;
    }

    h1 {
        font-size: 3rem;
        color: #333;
    }

    button {
        padding: 10px 20px;
        font-size: 1.2rem;
        border: none;
        border-radius: 5px;
        background-color: #007bff;
        color: #fff;
        cursor: pointer;
        transition: background-color 0.3s ease;
    }

    button:hover {
        background-color: #0056b3;
    }
</style>
