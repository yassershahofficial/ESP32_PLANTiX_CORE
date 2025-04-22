import {firebaseConfig} from './firebaseConfig.js';

// Initialize Firebase
firebase.initializeApp(firebaseConfig);
firebase.analytics();

//Reference to your data path
const db = firebase.database();
const plant = db.ref("Plant1"); 

function updateDisplay(data){
    document.getElementById("status").textContent = `Status : ${data.Status}`;
    document.getElementById("temperature").textContent = `Temperature : ${data.Temperature} °C`;
    document.getElementById("humidity").textContent = `Humidity : ${data.Humidity} %`;
};

function showError(error){
    document.getElementById("errormsg").textContent = `Error connecting to Firebase: ${error.message}`;
}

try{
    //update on changes
    plant.on("value", (snapshot) => {
        const data = snapshot.val();
        console.log("Firebase connected. Data received:", data);
        updateDisplay(data);
    }, 
    (error) => {
        console.error("Error connecting to Firebase:", error);
        showError(error);
    });
} catch(error){
    console.error("Error connecting to Firebase:", error);
    showError(error);
}

