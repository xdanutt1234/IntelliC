// Import necessary modules
import { window as tauriWindow } from '@tauri-apps/api/tauri';

// Function to adjust window height based on content height
async function adjustWindowHeight() {
  // Calculate content height
  const contentHeight = document.body.scrollHeight;

  // Update Tauri window height
  await tauriWindow.setSize({ height: contentHeight });
}

// Call the adjustWindowHeight function when the content changes or loads
document.addEventListener('DOMContentLoaded', adjustWindowHeight);
window.addEventListener('resize', adjustWindowHeight);
