let sizeDataSubmit = document.querySelector('.sizeDataSubmit');
let copyBlock = document.querySelector('.copyBlock');
let mainBlock = document.querySelector('.mainBlock .lineContainer');
let codeCpy = document.querySelector('.codeCpy');



sizeDataSubmit.addEventListener('click', function(event) {
    let x = document.querySelector('#x').value;
    let y = document.querySelector('#y').value;

    let size = 20;

    mainBlock.innerHTML = "";

    for (let i = 0; i < y; i+=1) {
        let lineString = `<div class="line">`;
        lineString += `<div class="node" style="width: ${size}px; height: ${size}px;" status = "1"></div>`.repeat(x);
        lineString += `</div>`;
        mainBlock.innerHTML += lineString;
        console.log(lineString);
    };
});

mainBlock.addEventListener('click', function(event) {
    let target = event.target;
    if (target.classList.contains('node')) {
        let currentStatus = target.getAttribute('status');
        let newStatus = currentStatus === '1' ? 0 : 1;
        target.setAttribute('status', newStatus.toString());
        target.style.backgroundColor = newStatus ? 'white' : 'black';
    }
});

codeCpy.addEventListener('click', function(event) {
    let nodes = document.querySelectorAll('.node');
    let stringArray = [];
    
    let x = parseInt(document.querySelector('#x').value);
    let y = parseInt(document.querySelector('#y').value);

    let mshtb = document.querySelector('.mshtb').value;
    mshtb = mshtb == "" ? 1 : parseInt(mshtb);

    for (let row = 0; row < y; row++) {
        let bitString = '';
        for (let col = 0; col < x; col++) {
            let index = row * x + col;
            if (index < nodes.length) {
                let status = nodes[index].getAttribute('status');
                bitString += status.repeat(mshtb);
            } else {
                bitString += '0'.repeat(mshtb);
            }
        }
        for (let scaleRow = 0; scaleRow < mshtb; scaleRow++) {
            for (let i = 0; i < bitString.length; i += 8) {
                let chunk = bitString.slice(i, i + 8);
                if (chunk.length < 8) {
                    chunk += '1'.repeat(8 - chunk.length);
                }
                stringArray.push("0b" + chunk + ", ");
            }
            stringArray.push('\n');
        }
    }
    
    let result = stringArray.join('');

    copyBlock.innerHTML = "";
    copyBlock.innerHTML = result;
    
    navigator.clipboard.writeText(result).then(() => {
        alert('Код скопирован в буфер обмена!');
    }).catch(err => {
        console.error('Ошибка копирования: ', err);
    });
});