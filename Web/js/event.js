//Utility Function
function AddSafeListener(id, event, handler){
    const element = document.getElementById(id);
    if(element){
        element.addEventListener(event, handler);
    }
}

const sections = document.querySelectorAll(".control-sys-sub");

if(sections.length != 0){
    sections.forEach(section => {
        const slider = section.querySelector("input[type='range']");
        const view = section.querySelector("p");

        view.textContent = slider.value;

        slider.addEventListener("input", () =>{
            view.textContent = slider.value;
        })      
    })
}


AddSafeListener("home-btn-1", "click", () => {
    window.location.href = "home.html";
});

AddSafeListener("home-btn-2", "click", () => {
    window.location.href = "home.html";
});

AddSafeListener("home-btn-3", "click", () => {
    window.location.href = "home.html";
});

AddSafeListener("menu-burger__redirect", "click", () => {
    window.location.href = "index.html"
});

AddSafeListener("home-favourite-opt-redirect", "click", () => {
    window.location.href = "plant.html"
});

AddSafeListener("home-favourite-opt-redirect-non", "click", () => {
    window.location.href = "no_plant.html"
});

AddSafeListener("home-favourite-opt-redirect-non2", "click", () => {
    window.location.href = "no_plant.html"
});

AddSafeListener("plant-title__redirect", "click", () => {
    window.location.href = "home.html"
});

AddSafeListener("plant-btns-view", "click", () => {
    // window.location.href = "control_system.html"
});

AddSafeListener("plant-btns-control", "click", () => {
    window.location.href = "control_system.html"
});

AddSafeListener("control-btns-update", "click", () => {
    // window.location.href = "plant.html"
});

AddSafeListener("control-btns-cancel", "click", () => {
    window.location.href = "plant.html"
})
