<!-- Courses.svelte -->
<script lang="ts">
    import { onMount } from "svelte";
    import Navbar from "../../resources/Navbar.svelte";

    interface Course {
        name: string;
    }

    let courses: Course[] = [];

    onMount(async () => {
        const response = await fetch("/courses");
        const data = await response.json();
        courses = data;
    });
</script>

<head>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
        }
        .courses_container {
            margin-top: 100px;
        }
    </style>
</head>

<Navbar />

<div class="courses_container">
    <h1>Courses List</h1>
    <ul>
        {#each courses as course}
            <li>{course.name}</li>
        {/each}
    </ul>
</div>
