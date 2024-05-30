<script>
 import Navbar from "../../resources/Navbar.svelte";
    import { onMount } from "svelte";
    import { db } from "../../javascript/firebase"; // Import the Firestore instance from your firebase.js
    import { collection, addDoc } from "firebase/firestore";
    import { goto } from "$app/navigation"; // Import the goto function from SvelteKit
    import { currentcourse } from "../../javascript/current_course";
    let title = '';
    let description = '';
    async function createCourse() {
        try {
            // Reference to the courses collection
            const coursesRef = collection(db, "courses");
            
            // Add a new document with the specified title and description
            await addDoc(coursesRef, {
                name: title,
                description: description
            });
            
            console.log("Course successfully created!");
            // Optionally navigate to another page or give feedback to the user
            goto("/list"); // Adjust the path as needed
        } catch (e) {
            console.error("Error adding document: ", e);
        }
    }
</script>

<Navbar />

<div class="form-container">
    <h2>Course Form</h2>
    <form>
        <div class="form-group">
            <label for="courseTitle">Course Title:</label>
            <input type="text" id="courseTitle" name="courseTitle" bind:value={title} required />
        </div>
        <div class="form-group">
            <label for="courseDescription">Course Description:</label>
            <textarea bind:value={description}
                id="courseDescription"
                name="courseDescription"
                rows="4"
                required
            ></textarea>
        </div>
        <div class="form-group">
            <button on:click={createCourse}>Submit</button>
        </div>
    </form>
</div>

<style>
    body {
        font-family: Arial, sans-serif;
        margin: 20px;
    }
    .form-container {
        max-width: 600px;
        margin: auto;
        margin-top: 160px;
        padding: 20px;
        border: 1px solid #ccc;
        border-radius: 10px;
    }
    .form-group {
        margin-bottom: 15px;
    }
    .form-group label {
        display: block;
        margin-bottom: 5px;
        font-weight: bold;
    }
    .form-group input,
    .form-group textarea {
        width: 100%;
        padding: 8px;
        box-sizing: border-box;
    }
    .form-group textarea {
        resize: vertical;
    }
    .form-group button {
        padding: 10px 15px;
        background-color: #007bff;
        color: white;
        border: none;
        border-radius: 5px;
        cursor: pointer;
    }
    .form-group button:hover {
        background-color: #0056b3;
    }
</style>
