//Utility Function
function AddSafeListener(id, event, handler){
    const element = document.getElementById(id);
    if(element){
        element.addEventListener(event, handler);
    }
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
})