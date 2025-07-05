import {firebaseConfig} from './firebaseConfig.js';

firebase.initializeApp(firebaseConfig);
firebase.analytics();

//Reference to your data path
const db = firebase.database();
const plant = db.ref("Plant1"); 
const requirements = db.ref("Plant1Requirements");

function updateDisplay(data){
    const keys = ["Temperature", "Humidity", "Soil_Moisture", "PH_Value"];

    keys.forEach((key) => {
        document.getElementById(key).textContent = Math.round(data[key]);
    });
    
}

function updateRequirementsDisplay(data){
    const sliders = document.querySelectorAll(".control-sys-sub input");
    const displays = document.querySelectorAll(".control-sys-sub p:last-child");

    const keys = ["StableTemperature", "StableHumidity", "StableSoil_Moisture", "StablePH_Value"];

    sliders.forEach((slider, index) => {
        slider.value = data[keys[index]];
        displays[index].textContent = data[keys[index]];
    });
}

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
};

try{
    //update only once
    requirements.once("value").then(snapshot => {
    const data = snapshot.val();

    console.log("Request Plant1Requirements. Data received:", data);
    updateRequirementsDisplay(data);
    },
    (error) => {
        console.error("Error loading thresholds", error);
        showError(error)
    });
} catch(error){
    console.error("Error loading thresholds", error);
    showError(error)
};

const update = document.getElementById("control-btns-update");

if(update){
    update.addEventListener("click", () => {
        const sliders = document.querySelectorAll(".control-sys-sub input");
        const keys = ["StableTemperature", "StableHumidity", "StableSoil_Moisture", "StablePH_Value"];
        const updatedRequirements = {};

        keys.forEach((key, index) => {
            updatedRequirements[key] = parseInt(sliders[index].value);
        });

        requirements.set(updatedRequirements)
            .then(() => {
                alert("Requirements Updated!");
            })
            .catch((error) => {
                alert("Requirements Failed to Update : " + error.message);
            });
    });
}

