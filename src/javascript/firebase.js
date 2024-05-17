// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAuth, GoogleAuthProvider, signInWithPopup } from "firebase/auth";
import { getFirestore } from "firebase/firestore";
import { getAnalytics } from "firebase/analytics";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyDFatfmcAG4vwkUv8sJC9P9P7UUL_9-7u0",
  authDomain: "intelliclassroom-17ea4.firebaseapp.com",
  projectId: "intelliclassroom-17ea4",
  storageBucket: "intelliclassroom-17ea4.appspot.com",
  messagingSenderId: "967692157875",
  appId: "1:967692157875:web:48e6f881557b195af501c6",
  measurementId: "G-GCYE2R3SNJ"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const auth = getAuth(app);
const provider = new GoogleAuthProvider();
const db = getFirestore(app);

const analytics = getAnalytics(app);
export default app;
export { auth };

