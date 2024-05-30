import { writable } from 'svelte/store';

export const currentproblem = writable(String);

const storedData = localStorage.getItem('problem');
if (storedData) {
  currentproblem.set(JSON.parse(storedData));
}

// Subscribe to changes in the store and update localStorage accordingly
currentproblem.subscribe(value => {
  localStorage.setItem('problem', JSON.stringify(value));
});