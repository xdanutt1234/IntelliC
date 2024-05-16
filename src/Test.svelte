<!-- App.svelte -->
<script lang="ts">
    import { onMount } from 'svelte';
  
    interface Course {
      name: string;
    }
  
    let courses: Course[] = [];
  
    onMount(async () => {
      const response = await fetch('/courses');
      const data = await response.json();
      courses = data;
    });
  </script>
  
  <nav id="navbar">
      <a href="#">Home</a>
      <a href="#">Courses</a>
      <a href="#">Performance Stats</a>
      <a href="#">Profile</a>
      <!-- Add more links/buttons as needed -->
  </nav>
  
  <h1>Courses List</h1>
  <ul>
    {#each courses as course}
      <li>{course.name}</li>
    {/each}
  </ul>
  
  <style>
    /* Basic CSS for navbar */
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
    }
    
    nav {
      background-color: #333;
      color: #fff;
      padding: 10px;
      text-align: center;
      position: fixed;
      top: -50px; /* Initially hides the navbar */
      width: 100%;
      transition: top 0.3s;
      z-index: 1000; /* Ensures the navbar stays on top of other elements */
    }
    
    nav a {
      color: #fff;
      text-decoration: none;
      padding: 10px 20px;
      margin: 0 5px;
      border-radius: 5px;
    }
    
    nav a:hover {
      background-color: #555;
    }
  </style>