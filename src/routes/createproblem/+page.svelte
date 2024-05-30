<script>
    import { onMount } from "svelte";
    import { db } from "../../javascript/firebase"; // Import the Firestore instance from your firebase.js
    import { collection, addDoc, query, doc, getDocs } from "firebase/firestore";
    import { goto } from "$app/navigation"; // Import the goto function from SvelteKit
    import { currentcourse } from "../../javascript/current_course";
    import Navbar from "../../resources/Navbar.svelte";
    let title = '';
    let summary = '';
    let description = '';
    let output = '';
    let problems = [];
    let courseId;

    async function getCourse() {
        const q = query(collection(db, "currentcourse"));
        const querySnapshot = await getDocs(q);
        problems = querySnapshot.docs.map((doc) => ({
            id: doc.id,
            ...doc.data(),
        }));
        courseId = problems[0].courseId;
    }
    async function createProblem() {
        try {
            await getCourse();
            // Reference to the courses collection
            const coursesRef = collection(db, "exercises");
            
            // Add a new document with the specified title and description
            await addDoc(coursesRef, {
                name: title,
                description: summary,
                edescription: description,
                solution: output,
                courseId: courseId
            });
            
            console.log("Course successfully created!");
            // Optionally navigate to another page or give feedback to the user
            goto("/problems"); // Adjust the path as needed
        } catch (e) {
            console.error("Error adding document: ", e);
        }
    }


</script>
<Navbar/>
<div class="form-container">
    <h2>Problem form</h2>
    <form>
        <div class="form-group">
            <label for="title">Title:</label>
            <input type="text" id="title" name="title" bind:value={title} required>
        </div>
        <div class="form-group">
            <label for="summary">Summary:</label>
            <input type="text" id="summary" name="summary" bind:value={summary} required>
        </div>
        <div class="form-group">
            <label for="description">Description:</label>
            <textarea id="description" name="description" bind:value={description}  rows="4" required></textarea>
        </div>
        <div class="form-group">
            <label for="output">Output:</label>
            <textarea id="output" name="output" bind:value={output} rows="4"  required></textarea>
        </div>
        <div class="form-group">
            <button on:click={createProblem}>Submit</button>
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
            margin-top: 100px;
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
        .form-group input, .form-group textarea {
            width: 100%;
            padding: 8px;
            box-sizing: border-box;
        }
        .form-group textarea {
            resize: vertical;
        }
        .form-group button {
            padding: 10px 15px;
            background-color: #007BFF;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }
        .form-group button:hover {
            background-color: #0056b3;
        }
        .output-container {
            margin-top: 20px;
            padding: 20px;
            border: 1px solid #ccc;
            border-radius: 10px;
            background-color: #f9f9f9;
        }
    </style>