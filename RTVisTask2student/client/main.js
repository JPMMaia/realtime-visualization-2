var g_application;

function onZoomInButtonClick()
{	
}
function onZoomOutButtonClick()
{	
}

function onPanLeftButtonClick()
{	
}
function onPanRightButtonClick()
{	
}
function onPanUpButtonClick()
{	
}
function onPanDownButtonClick()
{	
}

function setupUI()
{
	var zoomInButton = document.getElementById("zoom_in");
	zoomInButton.onclick = onZoomInButtonClick;
	
	var zoomOutButton = document.getElementById("zoom_out");
	zoomOutButton.onclick = onZoomOutButtonClick;
	
	var panLeftButton = document.getElementById("pan_left");
	panLeftButton.onclick = onPanLeftButtonClick;
	
	var panRightButton = document.getElementById("pan_right");
	panRightButton.onclick = onPanRightButtonClick;
	
	var panUpButton = document.getElementById("pan_up");
	panUpButton.onclick = onPanUpButtonClick;
	
	var panDownButton = document.getElementById("pan_down");
	panDownButton.onclick = onPanDownButtonClick;
}

function run()
{
	g_application.render();

    setTimeout(
        function()
        {
            window.requestAnimationFrame(run);
        },
        200
    );
}

function initializeApplication(websocket) 
{
	g_application = new Application(websocket);
	
	setupUI();
	run();
}