/*
DALY BMS to MQTT Project
https://github.com/softwarecrash/DALY-BMS-to-MQTT
This code is free for use without any waranty.
when copy code or reuse make a note where the codes comes from.
*/

const char HTML_MAIN[] PROGMEM = R"rawliteral(
<div class="row gx-0 mb-2" id="vcc_alert"style="display: none;">
    <div class="alert alert-danger" role="alert">
    <span><center><b>WARNING ESP VCC TO LOW</b></center></span>
    </div>
</div>
<figure class="text-center">
    <h2 id="devicename"></h2>
</figure>
<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Package:</div>
    </div>
    <div class="col">
        <div class="bg-light"><span id="packV"></span><span id="packA"></span><span id="packP"></span><span id="packSOC"></span></div>
    </div>
</div>
<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Remaining Capacity: </div>
    </div>
    <div class="col">
        <div class="bg-light"><span id="packRes"></span></div>
    </div>
</div>
<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Charge Cycles: </div>
    </div>
    <div class="col">
        <div class="bg-light"><span id="packCycles"></span></div>
    </div>
</div>
<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Temperature: </div>
    </div>
    <div class="col">
        <div class="bg-light"><span id="packTemp"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Cells Hi/Lo/Diff: </div>
    </div>
    <div class="col">
        <div class="bg-light"><span id="cellH"></span><span id="cellL"></span><span id="cellDiff"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">BMS Status: </div>
    </div>
    <div class="col">
        <div class="bg-light"><span id="status"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Charge MOS State: </div>
    </div>
    <div class="col">
        <div class="bg-light form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="chargeFetState" />
        </div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Discharge MOS State: </div>
    </div>
    <div class="col">
        <div class="bg-light form-check form-switch"><input class="form-check-input" type="checkbox" role="switch"
                id="disChargeFetState" /></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Balance State: </div>
    </div>
    <div class="col">
        <div class="bg-light form-check form-switch"><input class="form-check-input" type="checkbox" role="switch"
                id="cellBalanceActive" disabled /></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Relais Output: </div>
    </div>
    <div class="col">
        <div class="bg-light form-check form-switch"><input class="form-check-input" type="checkbox" role="switch"
                id="relaisOutputActive" disabled /></div>
    </div>
</div>

<div class="d-grid gap-2">
    <a class="btn btn-primary btn-block" href="/settings" role="button">Settings</a>
</div>

<script>
    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;
    window.addEventListener('load', onLoad);
    function initWebSocket() {
        console.log('Trying to open a WebSocket connection...');
        websocket = new WebSocket(gateway);
        websocket.onopen = onOpen;
        websocket.onclose = onClose;
        websocket.onerror = onError;
        websocket.onmessage = onMessage;
    }
    function onOpen(event) {
        console.log('Connection opened');
    }
    function onClose(event) {
        console.log('Connection closed');
        setTimeout(initWebSocket, 2000);
    }
        function onError(event) {
        console.log('Connection lost');
        setTimeout(initWebSocket, 2000);
    }
    function onMessage(event) {
        var data = JSON.parse(event.data);
        document.getElementById("devicename").innerHTML = 'Device: ' + data.Pack.Device_Name;
        document.getElementById("packV").innerHTML = data.Pack.Voltage == null ? 'No connection or Sleeping' : data.Pack.Voltage + 'V ';
        document.getElementById("packA").innerHTML = data.Pack.Current == null ? '' : data.Pack.Current + 'A  ';
        document.getElementById("packP").innerHTML = data.Pack.Power == null ? '' : Math.round(data.Pack.Power) + 'W  ';
        document.getElementById("packSOC").innerHTML = data.Pack.SOC == null ? '' : data.Pack.SOC + '%%';
        document.getElementById("packRes").innerHTML = data.Pack.Remaining_mAh == null ? '&#8205' : data.Pack.Remaining_mAh + 'mAh ';
        document.getElementById("packCycles").innerHTML = data.Pack.Cycles == null ? '&#8205' : data.Pack.Cycles + ' ';
        document.getElementById("packTemp").innerHTML = data.Pack.BMS_Temp == null ? '&#8205' : data.Pack.BMS_Temp + '°C ';
        document.getElementById("cellH").innerHTML = data.Pack.High_CellNr == null ? '&#8205' : data.Pack.High_CellNr + '| ' + data.Pack.High_CellV + 'V ';
        document.getElementById("cellL").innerHTML = data.Pack.Low_CellNr == null ? '&#8205' : data.Pack.Low_CellNr + '| ' + data.Pack.Low_CellV + 'V ';
        document.getElementById("cellDiff").innerHTML = data.Pack.Cell_Diff == null ? '&#8205' : data.Pack.Cell_Diff + 'mV ';
        document.getElementById("status").innerHTML = data.Pack.Status == null ? '&#8205' : data.Pack.Status;
        document.getElementById("chargeFetState").checked = data.Pack.ChargeFET;
        document.getElementById("disChargeFetState").checked = data.Pack.DischargeFET;
        document.getElementById("cellBalanceActive").checked = data.Pack.Balance_Active;
        document.getElementById("relaisOutputActive").checked = data.Pack.Relais_Active;        
        if(data.Pack.Relais_Manual){
            relaisOutputActive.removeAttribute("disabled")
        } else{
            relaisOutputActive.setAttribute('disabled', 'disabled');
        }
        if (data.Pack.ESP_VCC < 2.8) {
            document.getElementById("vcc_alert").style.display = '';
        }else{
            document.getElementById("vcc_alert").style.display = 'none';
        }

    }

    function onLoad(event) {
        initWebSocket();
        initButton();
    }

    function initButton() {
        document.getElementById('chargeFetState').addEventListener('click', ChargeFetSwitch);
        document.getElementById('disChargeFetState').addEventListener('click', DischargeFetSwitch);
        document.getElementById('relaisOutputActive').addEventListener('click', RelaisOutputSwitch);
    }

    function ChargeFetSwitch() {
        let switchVal;
        if (document.getElementById('chargeFetState').checked) { switchVal = 'chargeFetSwitch_on' }
        else { switchVal = 'chargeFetSwitch_off' }
        websocket.send(switchVal);
    }

    function RelaisOutputSwitch() {
        let switchVal;
        if (document.getElementById('relaisOutputActive').checked) { switchVal = 'relaisOutputSwitch_on' }
        else { switchVal = 'relaisOutputSwitch_off' }
        websocket.send(switchVal);
    }

    function DischargeFetSwitch() {
        let switchVal;
        if (document.getElementById('disChargeFetState').checked) {
			switchVal = 'dischargeFetSwitch_on';
			websocket.send(switchVal);
		}
        else { 
            switchVal = 'dischargeFetSwitch_off';
			var check = confirm('Are you sure to disable the DISCHARGE MOS?! You maybe create your own personal blackout!'); 
			if (check) {
				websocket.send(switchVal);
			} else {
				document.getElementById("disChargeFetState").checked = true;
			}
		}
    }
</script>
)rawliteral";