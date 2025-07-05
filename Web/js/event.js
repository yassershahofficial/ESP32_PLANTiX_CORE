//Utility Function
function AddSafeListener(id, event, handler){
    const element = document.getElementById(id);
    if(element){
        element.addEventListener(event, handler);
    }
}

function AddSelectorListener(selectorAll, event, handler){
    const elements = document.querySelectorAll(selectorAll);
    if(elements.length !== 0){
        elements.forEach(element => {
            element.addEventListener(event, (e) => handler(e, element));
        });
    }
}

document.addEventListener("DOMContentLoaded", () => {
    AddSelectorListener(".home-favourite-opt-sub[id^='home-favourite-opt-redirect-non']", "click", (event, element) => {
        const plantName = element.querySelector(".home-favourite-opt-sub-name p")?.textContent;
        const imageLink = element.querySelector(".home-favourite-opt-sub img")?.src;

        if(!plantName) return;
        if(!imageLink) return;

        const encodedName = encodeURIComponent(plantName);
        const encodedImg = encodeURIComponent(imageLink);
        window.location.href = `no_plant.html?plant=${encodedName}&img=${encodedImg}`;
    });

    AddSelectorListener(".home-category-opt-sub", "click", (event, element) => {
        const categoryTitle = element.querySelector(".home-category-opt-sub p")?.textContent;
        if(!categoryTitle) return;

        const encodedCategory = encodeURIComponent(categoryTitle);
        window.location.href = `categories.html?category=${encodedCategory}`;
    })

    if(window.location.pathname.endsWith("no_plant.html")) {
        const params = new URLSearchParams(window.location.search);
        const plantName = params.get("plant");
        const imageLink = params.get("img");
        const titleElement = document.querySelector(".plant-title-name h2");
        const imageElement = document.querySelector(".plant-title-wrapper img");

        if(titleElement){
            titleElement.textContent = plantName ? decodeURIComponent(plantName) : "Unknown Plant";
        }
        if(imageElement){
            imageElement.src = imageLink ? decodeURIComponent(imageLink) : "img/Plant1.png";
        }
    }
    else if(window.location.pathname.endsWith("categories.html")){
        const params = new URLSearchParams(window.location.search);
        const categoryTitle = params.get("category");
        const titleElement = document.querySelector(".categories-title h1");
        const icon = document.querySelector(".categories-title i");

        titleElement.textContent = categoryTitle ? decodeURIComponent(categoryTitle) : "Others";
    
        if (icon) {
            if (categoryTitle === "Outdoor") {
                icon.className = "fi fi-sr-daisy-alt";
            } else if (categoryTitle === "Indoor") {
                icon.className = "fi fi-sr-house-chimney-heart";
            } else if (categoryTitle === "Office") {
                icon.className = "fi fi-sr-city";
            } else {
                icon.className = "fi fi-sr-boxes";
            }
        }
    }
    else if(window.location.pathname.endsWith("home.html")) {
        const qrWrapper = document.getElementById("qr-reader-wrapper");
        const qrReader = document.getElementById("qr-reader");
        const qrBlank = document.getElementById("qr-fillspace");
        let scannerRunning = false;

        const html5QrCode = new Html5Qrcode("qr-reader");

        AddSafeListener("qr-close", "click", () => {
            if(scannerRunning){
                html5QrCode.stop().then(() => {
                    scannerRunning = false;
                    qrWrapper.style.visibility = "hidden";
                    qrBlank.style.visibility = "hidden";
                    document.body.style.overflow = "";
                });
            }
            else{
                qrWrapper.style.visibility = "hidden";
                qrBlank.style.visibility = "hidden";
                document.body.style.overflow = "";
            }
                
        });

        AddSafeListener("qr-scan-trigger", "click", () => {

            if(scannerRunning) return;
            qrWrapper.style.visibility = "visible";
            qrBlank.style.visibility = "visible";
            document.body.style.overflow = "hidden";

            html5QrCode.start(
                { 
                    facingMode: "environment" 
                },
                {
                    fps: 5, 
                    qrbox: function(viewfinderWidth, viewfinderHeight) {
                        const minEdge = Math.min(viewfinderWidth, viewfinderHeight);
                        return { 
                            width: minEdge * 0.75, 
                            height: minEdge * 0.75
                        };
                    }
                },
                qrCodeMessage => {
                    html5QrCode.stop().then(() => {
                        scannerRunning = false;
                        qrWrapper.style.visibility = "hidden";
                        qrBlank.style.visibility = "hidden"
                        document.body.style.overflow = "";
                        window.open(qrCodeMessage, "_blank");
                    }).catch(err => console.error("Stop error", err));
                },
                errorMessage => {
                    console.warn(`QR scan error: ${errorMessage}`);
                }
            ).then(() => {
                scannerRunning = true;
            }).catch(err => {
                console.error("Start scan error:", err);
            });
        });


    }
});


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

AddSafeListener("home-category-opt-sub1", "click", () => {
    window.location.href = "categories.html"
});
AddSafeListener("home-category-opt-sub2", "click", () => {
    window.location.href = "categories.html"
});
AddSafeListener("home-category-opt-sub3", "click", () => {
    window.location.href = "categories.html"
});
AddSafeListener("home-category-opt-sub4", "click", () => {
    window.location.href = "categories.html"
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
});
