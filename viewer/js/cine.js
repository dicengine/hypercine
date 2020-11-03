var frameBegin = 0;
var frameCount = 0;
var frameEnd = 0;
var frameWidth = 0;
var frameHeight = 0;

$(window).load(function(){
    execDir = __dirname;
    if(os.platform()=='win32'){
        execDir += '\\..\\build\\bin\\fetch_cine.exe';
    }else{
        execDir += '/../build/bin/fetch_cine';
    }
    $("#execTextSpan").text(execDir);
    $("#wdTextSpan").text(os.homedir());
});

//these three transform a string into a file object
var getFileBlob = function (url, cb) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", url);
    xhr.responseType = "blob";
    xhr.addEventListener('load', function() {
        cb(xhr.response);
    });
    xhr.send();
};
var blobToFile = function (blob, name) {
    blob.lastModifiedDate = new Date();
    if(os.platform()=='win32'){
        blob.name = name.split('\\').pop();
    }else{
        blob.name = name.split('/').pop();
    }
    blob.path = name;
    return blob;
};
var getFileObject = function(filePathOrUrl, cb) {
    getFileBlob(filePathOrUrl, function (blob) {
        cb(blobToFile(blob, filePathOrUrl));
    });
};

$("#advanceFrame").on("click",function () {
    var frame = Number($("#frameSelect").val());
    frame++;
    if(frame>frameEnd) return;
    $("#frameSelect").val(frame);
    updateView(frame);
});

$("#decrementFrame").on("click",function () {
    var frame = Number($("#frameSelect").val());
    frame--;
    if(frame<frameBegin) return;
    $("#frameSelect").val(frame);
    updateView(frame);
});

$("#cineInput").on("click",function () {
    this.value = null;
});
$("#cineInput").change(function (evt) {
    var tgt = evt.target || window.event.srcElement,
    file = tgt.files[0];
    $("#cineTextSpan").text(file.path);
    updateCineInfo(file.path);
});

$("#frameSelect").keypress(function(event) { 
    if (event.keyCode === 13) { 
        updateView(Number($(this).val()));
    }
}); 

$("#execInput").on("click",function () {
    this.value = null;
});
$("#execInput").change(function (evt) {
    var tgt = evt.target || window.event.srcElement,
    file = tgt.files[0];
    $("#execTextSpan").text(file.path);
});

$("#wdInput").on("click",function () {
    this.value = null;
});
$("#wdInput").change(function (evt) {
    var tgt = evt.target || window.event.srcElement,
    file = tgt.files[0];
    $("#wdTextSpan").text(file.path);
});

function updateView(frame){
    if($("#wdTextSpan").text().length == 0){
        alert('working directory needs to be set');
        return;
    }
    if($("#execTextSpan").text().length == 0){
        alert('hypercine exectuable needs to be set');
        return;
    }
    if($("#cineTextSpan").text().length == 0){
        alert('cine file needs to be selected');
        return;
    }
    if(frame<frameBegin || frame>frameEnd){
        alert('invalid frame selected ' + frame);
        return;
    }
    var execPath = $("#execTextSpan").text();
    var wd = $("#wdTextSpan").text();
    var displayImageFileName = wd;
    if(os.platform()=='win32'){ displayImageFileName += '\\'; } else { displayImageFileName += '/'}
    displayImageFileName += ".display_image.png";
    
    args = [];
    // create the arguments to the fetch_cine exec:
    args.push($("#cineTextSpan").text());
    args.push(frame);
    args.push(displayImageFileName);
    
    // call the filter exec
    var childProcess = require('child_process');
    var readline      = require('readline');
    var proc = childProcess.spawn(execPath,args,{cwd:wd});

    proc.on('error', function(){
        alert('Error: fetch_cine failed, invalid executable: ' + execPath);
    });
    proc.on('close', (code) => {
        console.log(`fetch_cine exited with code ${code}`);
        if(code!=0){
            alert('Error: fetch_cine failed');
        }
        else{
            // load new preview images
            fs.stat(displayImageFileName, function(err, stat) {
                if(err == null) {
                    getFileObject(displayImageFileName, function (fileObject) {
                        drawImage(displayImageFileName);
                    });
                }
            });
        }
    });
    readline.createInterface({
        input     : proc.stdout,
        terminal  : false
    }).on('line', function(line) {
        console.log(line);
    });
}

function drawImage(image){
    // clear the divs and clear the plots
    $('#viewer').empty();
    var layout = {
            xaxis: {
                range: [0,frameWidth],
                showgrid: false,
                zeroline: false,
                showline: false,
            },
            yaxis: {
                range: [frameHeight,0],
                scaleanchor: 'x',
                showgrid: false,
                zeroline: false,
                showline: false,
            },
            images: [
                {
                    source: image,
                    xref: 'x',
                    yref: 'y',
                    x: 0,
                    y: 0,
                    sizex: frameWidth,
                    sizey: frameHeight,
                    layer: 'below',
                }],
    };
    
    var config = {
            modeBarButtonsToAdd: ['drawline',
                'drawopenpath',
                'drawclosedpath',
                'drawcircle',
                'drawrect',
                'eraseshape']
    }
    
    
    var divID = "divPlotly";
    $("#viewer").append('<div id="' + divID + '" class="plot_div" style="height:100%; width:100%; float:left;" ></div>');
    Plotly.plot(document.getElementById(divID),[],layout,config);
}

function updateCineInfo(path){
    if($("#execTextSpan").text().length == 0){
        alert('hypercine exectuable needs to be set');
        return;
    }
    var execPath = $("#execTextSpan").text();
    args = [];
    // create the arguments to the fetch_cine exec:
    args.push(path);
    
    // call the filter exec
    var childProcess = require('child_process');
    var readline      = require('readline');
    var proc = childProcess.spawn(execPath,args);

    proc.on('error', function(){
        alert('Error: fetch_cine failed, invalid executable: ' + execPath);
    });
    proc.on('close', (code) => {
        console.log(`fetch_cine exited with code ${code}`);
        if(code!=0){
            alert('Error: fetch_cine failed');
        }else{
            $("#frameSelect").val(frameBegin);
            updateView(frameBegin);
        }
    });
    readline.createInterface({
        input     : proc.stdout,
        terminal  : false
    }).on('line', function(line) {
        if(line.includes("BUFFER_OUT")&&line.includes("FIRST_FRAME_ID")){
            console.log('setting frameBegin to ' + line.split(' ').pop());
            frameBegin = Number(line.split(' ').pop());
            $("#frameBeginTextSpan").text(frameBegin);
        }
        if(line.includes("BUFFER_OUT")&&line.includes("FRAME_COUNT")){
            console.log('setting frameCount to ' + line.split(' ').pop());
            frameCount = Number(line.split(' ').pop());
            frameEnd = frameBegin + frameCount - 1;
            $("#frameEndTextSpan").text(frameEnd);
        }
        if(line.includes("BUFFER_OUT")&&line.includes("FRAME_WIDTH")){
            console.log('setting frameWidth to ' + line.split(' ').pop());
            frameWidth = Number(line.split(' ').pop());
            $("#frameWidthTextSpan").text(frameWidth);
        }
        if(line.includes("BUFFER_OUT")&&line.includes("FRAME_HEIGHT")){
            console.log('setting frameHeight to ' + line.split(' ').pop());
            frameHeight = Number(line.split(' ').pop());
            $("#frameHeightTextSpan").text(frameHeight);
        }
        console.log(line);
    });
}

