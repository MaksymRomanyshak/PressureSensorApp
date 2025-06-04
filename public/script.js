async function fetchData() {
  try {
    let response = await fetch("/data");
    let data = await response.json();
    let pressure = Math.round(data.pressure);
    pressure = Math.max(pressure, 0);

    updatePressureDisplay(pressure);
  } catch (error) {
    console.error("Error fetching data:", error);
  }
}

function updatePressureDisplay(pressure) {
  let pressureElement = document.getElementById("pressure");
  let fromVal = parseInt(document.querySelector(".from").value);
  let toVal = parseInt(document.querySelector(".to").value);

  if (!isNaN(pressure)) {
    pressureElement.textContent = `${pressure} psi`;

    // Check if pressure is out of range
    pressure < fromVal || pressure > toVal
      ? (pressureElement.style.backgroundColor = "#C7923E")
      : (pressureElement.style.backgroundColor = "#d9d9d9");
  } else {
    pressureElement.textContent = "--";
  }
}

// Add event listeners to update the display when inputs change
document.querySelector(".from").addEventListener("onChange", () => fetchData());
document.querySelector(".to").addEventListener("onChange", () => fetchData());

fetchData();
setInterval(fetchData, 1000); // Fetch data every 3 seconds
