const char CSS_STYLE[] PROGMEM = R"=====(
.container{position:relative;width:79%;margin:20px;box-sizing:border-box}.column,.columns{width:100%;float:left}.card{margin-top:2%;border-radius:6px;box-shadow:0 4px 4px rgba(204,197,185,0.5);padding-left:20px;padding-right:20px;margin-bottom:10px;min-width:150px;color:#fff}.card-slider{padding-bottom:10px}.turquoise{background:#1abc9c;border-bottom:#16a085 3px solid}.emerald{background:#2ecc71;border-bottom:#27ae60 3px solid}.peterriver{background:#3498db;border-bottom:#2980b9 3px solid}.wetasphalt{background:#34495e;border-bottom:#2c3e50 3px solid}.sunflower{background:#f1c40f;border-bottom:#e6bb0f 3px solid}.carrot{background:#e67e22;border-bottom:#d35400 3px solid}.alizarin{background:#e74c3c;border-bottom:#c0392b 3px solid}.label{box-sizing:border-box;white-space:nowrap;border-radius:.2em;padding:.12em .4em .14em;text-align:center;color:#fff;font-weight:700;line-height:1;margin-bottom:5px;display:inline-block;white-space:nowrap;vertical-align:baseline;position:relative;top:-.15em;background-color:#999;margin-bottom:10px}.label-wrap{width:90%;white-space:pre-wrap;word-wrap:break-word}.label.color-blue{background-color:#6f9ad1}.label.color-red{background-color:#d37c7c}.label.color-green{background-color:#9bc268}.label.color-orange{background-color:#dea154}.label.color-yellow{background-color:#e9d641}.label.color-purple{background-color:#9f83d1}@media(min-width:400px){.container{width:84%}}@media(min-width:630px){.container{width:98%}.column,.columns{margin-right:2%}.column:first-child,.columns:first-child{margin-left:0}.one.column,.one.columns{width:4.66666666667%}.two.columns{width:13.3333333333%}.three.columns{width:22%}.four.columns{width:30.6666666667%}.five.columns{width:39.3333333333%}.six.columns{width:48%}.seven.columns{width:56.6666666667%}.eight.columns{width:65.3333333333%}.nine.columns{width:74.0%}.ten.columns{width:82.6666666667%}.eleven.columns{width:91.3333333333%}.twelve.columns{width:100%;margin-left:0}.one-third.column{width:30.6666666667%}.two-thirds.column{width:65.3333333333%}.one-half.column{width:48%}.offset-by-one.column,.offset-by-one.columns{margin-left:8.66666666667%}.offset-by-two.column,.offset-by-two.columns{margin-left:17.3333333333%}.offset-by-three.column,.offset-by-three.columns{margin-left:26%}.offset-by-four.column,.offset-by-four.columns{margin-left:34.6666666667%}.offset-by-five.column,.offset-by-five.columns{margin-left:43.3333333333%}.offset-by-six.column,.offset-by-six.columns{margin-left:52%}.offset-by-seven.column,.offset-by-seven.columns{margin-left:60.6666666667%}.offset-by-eight.column,.offset-by-eight.columns{margin-left:69.3333333333%}.offset-by-nine.column,.offset-by-nine.columns{margin-left:78.0%}.offset-by-ten.column,.offset-by-ten.columns{margin-left:86.6666666667%}.offset-by-eleven.column,.offset-by-eleven.columns{margin-left:95.3333333333%}.offset-by-one-third.column,.offset-by-one-third.columns{margin-left:34.6666666667%}.offset-by-two-thirds.column,.offset-by-two-thirds.columns{margin-left:69.3333333333%}.offset-by-one-half.column,.offset-by-one-half.columns{margin-left:52%}}html{font-size:62.5%}body{margin:0;font-size:1.5em;line-height:1.0;font-weight:400;font-family:'Open Sans',sans-serif;color:#222;background-color:#ecf0f1}h1,h2,h3,h4,h5,h6{margin-top:0;margin-bottom:.5rem;font-weight:300}h1{font-size:4.0rem;line-height:1.2;letter-spacing:-.1rem}h2{font-size:3.6rem;line-height:1.25;letter-spacing:-.1rem}h3{font-size:3.0rem;line-height:1.3;letter-spacing:-.1rem}h4{font-size:2.4rem;line-height:1.35;letter-spacing:-.08rem}h5{font-size:1.8rem;line-height:1.5;letter-spacing:-.05rem}h6{font-size:1.5rem;line-height:1.6;letter-spacing:0}@media(min-width:630px){h1{font-size:5.0rem}h2{font-size:4.2rem}h3{font-size:3.6rem}h4{font-size:3.0rem}h5{font-size:2.0rem}h6{font-size:1.5rem}}p{margin-top:0}a{color:#1eaedb}a:hover{color:#0fa0ce}button{display:inline-block;padding:10px;border-radius:3px;color:#fff;background-color:#999}#mainHeader{display:inline-block}#conStatus{position:inherit;font-size:.75em}button,.button{margin-bottom:1rem}.u-full-width{width:100%;box-sizing:border-box}.u-max-full-width{max-width:100%;box-sizing:border-box}.u-pull-right{float:right}.u-pull-left{float:left}.tcenter{text-align:center}hr{margin-top:.5rem;margin-bottom:1.2rem;border-width:0;border-top:1px solid #e1e1e1}.container:after,.row:after,.u-cf{content:"";display:table;clear:both}.control{background-color:#ddd;background-image:linear-gradient(hsla(0,0%,0%,.1),hsla(0,0%,100%,.1));border-radius:50%;box-shadow:inset 0 1px 1px 1px hsla(0,0%,100%,.5),0 0 1px 1px hsla(0,0%,100%,.75),0 0 1px 2px hsla(0,0%,100%,.25),0 0 1px 3px hsla(0,0%,100%,.25),0 0 1px 4px hsla(0,0%,100%,.25),0 0 1px 6px hsla(0,0%,0%,.75);height:9em;margin:3em auto;position:relative;width:9em}.control ul{height:100%;padding:0;transform:rotate(45deg)}.control li{border-radius:100% 0 0 0;box-shadow:inset -1px -1px 1px hsla(0,0%,100%,.5),0 0 1px hsla(0,0%,0%,.75);display:inline-block;height:50%;overflow:hidden;width:50%}.control ul li:nth-child(2){transform:rotate(90deg)}.control ul li:nth-child(3){transform:rotate(-90deg)}.control ul li:nth-child(4){transform:rotate(180deg)}.control ul a{height:200%;position:relative;transform:rotate(-45deg);width:200%}.control a:hover,.control a:focus{background-color:hsla(0,0%,100%,.25)}.control a{border-radius:50%;color:#333;display:block;font:bold 1em/3 sans-serif;text-align:center;text-decoration:none;text-shadow:0 1px 1px hsla(0,0%,100%,.4);transition:.15s}.control .confirm{background-color:#ddd;background-image:linear-gradient(hsla(0,0%,0%,.15),hsla(0,0%,100%,.25));box-shadow:inset 0 1px 1px 1px hsla(0,0%,100%,.5),0 0 1px 1px hsla(0,0%,100%,.25),0 0 1px 2px hsla(0,0%,100%,.25),0 0 1px 3px hsla(0,0%,100%,.25),0 0 1px 4px hsla(0,0%,100%,.25),0 0 1px 6px hsla(0,0%,0%,.85);left:50%;line-height:3;margin:-1.5em;position:absolute;top:50%;width:3em}.control .confirm:hover,.control .confirm:focus{background-color:#eee}.switch{display:inline-block !important;background-color:#bebebe;border-radius:4px;box-shadow:inset 0 0 6px rgba(0,0,0,0.3);color:#fff;cursor:pointer;display:block;font-size:14px;height:26px;margin-bottom:12px;position:relative;width:60px;-webkit-transition:background-color .2s ease-in-out;-moz-transition:background-color .2s ease-in-out;-o-transition:background-color .2s ease-in-out;-ms-transition:background-color .2s ease-in-out;transition:background-color .2s ease-in-out}.switch.checked{background-color:#76d21d}.switch input[type="checkbox"]{display:none;cursor:pointer;height:10px;left:12px;position:absolute;top:8px;width:10px}.in{position:absolute;top:8px;left:12px;-webkit-transition:left .08s ease-in-out;-moz-transition:left .08s ease-in-out;-o-transition:left .08s ease-in-out;-ms-transition:left .08s ease-in-out;transition:left .08s ease-in-out}.switch.checked div{left:38px}.switch .in:before{background:#fff;background:-moz-linear-gradient(top,#fff 0,#f0f0f0 100%);background:-webkit-gradient(linear,left top,left bottom,color-stop(0,#fff),color-stop(100%,#f0f0f0));background:-webkit-linear-gradient(top,#fff 0,#f0f0f0 100%);background:-o-linear-gradient(top,#fff 0,#f0f0f0 100%);background:-ms-linear-gradient(top,#fff 0,#f0f0f0 100%);background:linear-gradient(to bottom,#fff 0,#f0f0f0 100%);border:1px solid #fff;border-radius:2px;box-shadow:0 0 4px rgba(0,0,0,0.3);content:'';height:18px;position:absolute;top:-5px;left:-9px;width:26px}.switch .in:after{background:#f0f0f0;background:-moz-linear-gradient(top,#f0f0f0 0,#fff 100%);background:-webkit-gradient(linear,left top,left bottom,color-stop(0,#f0f0f0),color-stop(100%,#fff));background:-webkit-linear-gradient(top,#f0f0f0 0,#fff 100%);background:-o-linear-gradient(top,#f0f0f0 0,#fff 100%);background:-ms-linear-gradient(top,#f0f0f0 0,#fff 100%);background:linear-gradient(to bottom,#f0f0f0 0,#fff 100%);border-radius:10px;content:'';height:12px;margin:-1px 0 0 -1px;position:absolute;width:12px}.rkmd-slider{display:block;position:relative;font-size:16px;font-family:'Roboto',sans-serif}.rkmd-slider input[type="range"]{overflow:hidden;position:absolute;width:1px;height:1px;opacity:0}.rkmd-slider input[type="range"]+.slider{display:block;position:relative;width:100%;height:27px;border-radius:13px;background-color:#bebebe}@media(pointer:fine){.rkmd-slider input[type="range"]+.slider{height:4px;border-radius:0}}.rkmd-slider input[type="range"]+.slider .slider-fill{display:block;position:absolute;width:0;height:100%;user-select:none;z-index:1}.rkmd-slider input[type="range"]+.slider .slider-handle{cursor:pointer;position:absolute;top:12px;left:0;width:15px;height:15px;margin-left:-8px;border-radius:50%;transition:all .2s ease;user-select:none;z-index:2}@media(pointer:fine){.rkmd-slider input[type="range"]+.slider .slider-handle{top:-5.5px}}.rkmd-slider input[type="range"]:disabled+.slider{background-color:#b0b0b0 !important}.rkmd-slider input[type="range"]:disabled+.slider .slider-fill,.rkmd-slider input[type="range"]:disabled+.slider .slider-handle{cursor:default !important;background-color:#b0b0b0 !important}.rkmd-slider input[type="range"]:disabled+.slider .slider-fill .slider-label,.rkmd-slider input[type="range"]:disabled+.slider .slider-handle .slider-label{display:none;background-color:#b0b0b0 !important}.rkmd-slider input[type="range"]:disabled+.slider .slider-fill.is-active,.rkmd-slider input[type="range"]:disabled+.slider .slider-handle.is-active{top:-5.5px;width:15px;height:15px;margin-left:-8px}.rkmd-slider input[type="range"]:disabled+.slider .slider-fill.is-active .slider-label,.rkmd-slider input[type="range"]:disabled+.slider .slider-handle.is-active .slider-label{display:none;border-radius:50%;transform:none}.rkmd-slider input[type="range"]:disabled+.slider .slider-handle:active{box-shadow:none !important;transform:scale(1) !important}.rkmd-slider.slider-discrete .slider .slider-handle{position:relative;z-index:1}.rkmd-slider.slider-discrete .slider .slider-handle .slider-label{position:absolute;top:-17.5px;left:4px;width:30px;height:30px;-webkit-transform-origin:50% 100%;transform-origin:50% 100%;border-radius:50%;-webkit-transform:scale(1.0) rotate(-45deg);transform:scale(1.0) rotate(-45deg);-webkit-transition:all .2s ease;transition:all .2s ease}@media(pointer:fine){.rkmd-slider.slider-discrete .slider .slider-handle .slider-label{left:-2px;-webkit-transform:scale(0.5) rotate(-45deg);transform:scale(0.5) rotate(-45deg)}}.rkmd-slider.slider-discrete .slider .slider-handle .slider-label span{position:absolute;top:7px;left:0;width:100%;color:#fff;font-size:16px;text-align:center;-webkit-transform:rotate(45deg);transform:rotate(45deg);opacity:0;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}@media(pointer:fine){.rkmd-slider.slider-discrete .slider .slider-handle .slider-label span{font-size:12px}}.rkmd-slider.slider-discrete .slider .slider-handle.is-active{top:0;margin-left:-2px;width:4px;height:4px}.rkmd-slider.slider-discrete .slider .slider-handle.is-active .slider-label{top:-15px;left:-2px;border-radius:15px 15px 15px 0;-webkit-transform:rotate(-45deg) translate(23px,-25px);transform:rotate(-45deg) translate(23px,-25px)}.rkmd-slider.slider-discrete .slider .slider-handle.is-active .slider-label span{opacity:1}.rkmd-slider.slider-discrete.slider-turquoise .slider-label{background-color:#16a085}.rkmd-slider.slider-discrete.slider-emerald .slider-label{background-color:#27ae60}.peterriver{background:#3498db;border-bottom:#2980b9 3px solid}.rkmd-slider.slider-discrete.slider-peterriver .slider-label{background-color:#2980b9}.wetasphalt{background:#34495e;border-bottom:#2c3e50 3px solid}.rkmd-slider.slider-discrete.slider-wetasphalt .slider-label{background-color:#2c3e50}.sunflower{background:#f1c40f;border-bottom:#e6bb0f 3px solid}.rkmd-slider.slider-discrete.slider-sunflower .slider-label{background-color:#e6bb0f}.carrot{background:#e67e22;border-bottom:#d35400 3px solid}.rkmd-slider.slider-discrete.slider-carrot .slider-label{background-color:#d35400}.alizarin{background:#e74c3c;border-bottom:#c0392b 3px solid}.rkmd-slider.slider-discrete.slider-alizarin .slider-label{background-color:#c0392b}input{margin:0 auto 1.2rem auto;padding:2px 5px;width:100%;box-sizing:border-box;border:0;border-radius:4px;box-shadow:inset 0 0 6px rgba(0,0,0,0.3);background:rgba(255,255,255,0.8)}input[id^="num"]{max-width:6em;width:auto;text-align:right;font-weight:bold;font-size:115%}
)=====";

const uint8_t CSS_STYLE_GZIP[2816] PROGMEM = { 31,139,8,0,115,176,253,91,2,255,197,26,105,111,172,56,242,175,176,137,162,151,214,2,107,206,110,104,141,180,31,247,219,74,59,31,71,51,146,1,19,80,104,232,229,152,36,175,197,127,95,31,28,190,72,119,142,209,190,126,73,26,187,170,92,183,203,101,236,180,169,123,88,214,168,189,156,155,174,236,203,166,142,91,84,193,190,252,19,29,95,202,172,47,226,125,244,112,60,193,246,169,172,99,23,156,95,143,73,243,106,117,229,207,178,126,138,147,166,205,80,107,225,145,209,78,155,106,56,213,230,244,183,187,48,100,7,128,135,99,94,53,176,143,43,148,247,24,12,182,217,133,145,179,250,230,28,187,15,199,137,74,11,179,114,232,226,112,94,162,128,89,243,18,3,195,63,191,210,159,246,41,129,143,46,240,77,39,218,155,206,33,48,129,29,236,142,103,152,101,152,21,139,80,103,252,205,35,109,249,84,76,67,211,122,73,211,247,205,9,243,68,134,240,243,196,98,64,158,49,219,77,27,223,231,121,206,120,180,186,170,204,136,86,38,106,28,238,104,247,67,251,223,161,41,59,116,73,96,250,252,212,54,67,157,197,247,14,76,210,40,61,46,74,161,8,247,78,8,193,33,48,60,44,65,215,96,154,163,141,78,168,133,85,38,224,186,40,77,247,142,140,235,238,33,10,1,143,123,70,61,106,91,108,156,86,64,247,252,232,144,37,10,122,116,0,73,196,163,191,160,30,118,231,2,86,189,132,238,71,1,82,208,83,15,5,194,234,221,80,99,91,190,72,139,231,78,234,131,92,198,70,97,146,128,156,199,198,106,109,27,113,97,20,238,145,235,202,168,153,23,248,64,88,24,86,229,79,216,150,181,136,188,247,83,79,209,119,10,188,200,77,120,228,10,38,168,186,104,221,246,248,82,148,61,178,186,51,76,81,92,55,47,45,60,75,238,104,187,232,52,187,84,108,59,248,201,176,125,242,203,193,191,143,61,122,237,45,204,220,83,29,167,168,198,166,225,252,232,152,227,208,178,94,16,117,195,61,0,199,10,135,153,85,176,103,71,242,201,0,187,96,86,118,231,10,190,197,101,77,33,147,170,73,159,117,12,98,227,247,101,10,171,105,225,4,118,136,32,28,213,0,38,1,102,217,78,128,57,93,21,103,77,44,70,81,164,9,140,73,93,22,89,105,138,225,8,135,48,207,198,185,69,116,250,248,130,53,69,191,197,73,139,224,179,69,158,39,124,155,46,130,101,24,248,24,153,151,14,243,8,102,142,8,218,162,76,3,153,121,251,116,159,138,144,79,45,66,181,6,54,74,82,55,60,136,176,77,11,235,39,29,11,25,130,78,224,139,192,111,168,194,222,173,1,70,81,22,250,18,191,231,161,61,87,58,202,81,126,240,176,112,255,60,161,172,132,143,107,158,193,46,125,126,221,93,236,53,227,178,241,131,255,48,170,208,161,167,135,142,14,15,106,174,157,204,56,101,188,5,32,206,203,182,235,173,180,40,171,108,1,230,7,103,68,154,60,193,104,55,53,90,104,175,223,231,92,238,219,225,242,111,143,23,233,95,26,57,219,123,182,183,252,35,16,5,54,149,4,227,18,254,242,102,104,165,113,15,216,2,245,28,59,176,12,18,137,228,187,242,85,102,145,104,167,67,127,162,90,154,8,66,145,58,141,66,9,38,12,68,242,117,169,168,96,239,219,128,200,165,208,63,184,18,253,74,195,68,228,72,234,121,65,149,34,36,221,49,85,187,88,125,81,182,217,4,187,161,50,108,16,6,214,137,112,178,96,132,28,222,3,114,17,138,234,174,201,243,14,245,86,242,102,9,190,160,25,237,4,231,57,72,190,177,98,172,94,98,106,71,69,58,206,94,98,117,197,224,124,201,220,24,23,105,185,161,64,128,243,57,83,63,44,162,123,190,189,33,18,231,154,166,126,88,164,228,123,91,66,173,30,108,106,71,69,58,129,43,34,115,46,102,110,140,139,4,66,176,37,18,31,15,230,198,184,68,43,218,18,138,139,27,83,63,44,82,218,31,104,80,113,38,213,138,213,111,8,117,8,55,133,170,54,52,36,69,39,79,45,10,182,196,146,131,208,124,103,238,86,79,82,34,214,124,111,242,86,3,72,241,109,110,79,169,254,53,22,253,169,186,208,210,5,87,75,40,14,93,59,120,24,147,38,123,155,64,99,112,92,103,29,155,212,22,66,97,99,3,161,240,193,187,30,123,206,225,169,172,222,226,31,255,62,163,218,248,21,214,221,15,179,195,191,177,183,182,101,62,151,77,46,41,6,213,237,55,205,65,238,140,133,99,22,174,89,120,102,225,155,69,96,22,33,127,142,0,82,45,99,7,45,230,140,231,196,3,0,147,224,68,195,185,188,85,184,119,143,21,234,113,33,71,107,29,82,243,225,250,9,67,141,133,203,97,122,118,168,193,12,182,80,61,1,85,179,168,183,133,233,115,152,174,237,107,48,53,139,130,3,197,13,46,188,157,14,42,174,14,53,160,168,161,128,26,168,168,161,140,10,54,43,24,65,229,1,149,94,84,166,111,187,26,45,133,138,252,222,132,27,8,58,1,27,12,143,227,89,112,143,17,94,38,111,114,16,68,89,50,194,184,104,200,49,106,26,5,57,4,41,26,147,1,123,79,125,209,150,226,243,41,192,97,167,96,254,148,224,9,39,72,125,177,61,222,159,112,33,247,47,4,201,177,82,183,192,120,143,139,189,95,123,216,15,221,122,24,47,235,2,7,72,207,197,156,189,199,49,55,49,106,218,19,195,82,33,79,20,96,15,86,62,84,21,179,6,95,92,108,28,224,7,235,4,95,121,20,242,120,11,218,153,160,208,218,243,194,78,250,244,251,50,67,82,203,133,111,1,244,236,168,116,81,14,79,99,209,242,54,99,142,39,73,70,157,101,214,62,227,14,204,143,4,201,153,207,125,198,61,114,200,103,92,75,232,24,230,120,21,211,110,155,151,249,235,96,165,249,133,0,96,6,226,187,187,229,12,214,195,164,66,199,180,66,176,197,210,246,5,163,210,54,149,238,40,145,101,188,197,203,19,124,66,49,49,43,36,103,21,236,32,152,246,99,209,85,240,17,152,224,129,252,183,157,157,185,14,16,237,146,161,157,228,84,193,172,116,214,9,41,107,156,195,13,96,16,9,231,31,153,72,176,51,1,7,34,79,239,185,121,87,51,239,114,243,222,149,121,255,202,124,40,204,79,171,31,167,12,18,45,134,141,61,124,152,134,67,223,28,183,250,79,17,241,229,73,251,198,80,93,230,36,68,124,114,14,73,112,236,241,65,175,203,155,246,20,183,13,14,33,244,232,7,25,122,218,173,152,85,121,17,213,75,8,24,132,91,160,106,217,34,18,88,55,104,89,149,80,155,58,38,158,137,69,73,210,33,77,148,184,40,179,12,213,147,144,1,120,224,133,196,220,198,117,95,176,83,218,163,187,187,40,226,69,64,20,79,198,241,52,56,214,53,36,95,131,228,28,84,36,56,27,193,165,70,80,251,14,202,202,204,26,147,176,4,107,165,55,37,98,147,27,200,155,20,231,64,37,210,52,238,198,145,185,168,209,51,5,40,174,143,22,179,48,123,144,108,138,227,186,202,12,7,157,254,225,25,92,37,162,182,116,232,72,134,210,166,133,84,204,26,23,81,108,112,233,81,110,121,138,191,99,186,96,250,177,157,160,91,25,38,95,240,89,252,244,77,25,37,80,83,10,214,207,238,155,51,136,251,127,205,32,7,28,95,172,82,197,198,229,43,18,111,206,38,22,171,72,23,151,132,9,222,12,134,158,181,194,8,214,116,114,230,115,202,108,8,217,15,151,241,13,119,188,71,8,141,118,247,82,246,105,161,221,208,141,191,149,167,115,211,246,176,238,53,37,65,130,200,71,202,249,190,216,253,158,45,6,168,50,104,7,28,43,131,124,108,111,199,87,28,233,208,118,248,251,185,41,169,203,170,222,62,213,70,132,254,28,187,161,218,25,119,73,255,124,35,15,135,164,236,193,197,116,242,92,246,22,231,213,178,100,134,237,118,6,130,29,178,48,225,102,232,143,214,169,249,249,49,132,230,131,244,187,15,193,127,0,118,54,175,157,22,40,125,214,118,40,247,97,230,58,217,12,104,148,245,121,232,127,235,223,206,232,151,59,138,132,173,121,247,251,226,30,52,119,72,198,90,54,52,172,95,214,4,17,204,32,184,240,1,207,204,53,25,105,214,150,245,101,27,112,37,166,49,27,153,52,240,89,225,125,91,109,64,53,183,80,234,174,3,93,3,144,245,111,100,229,159,23,118,166,62,16,241,39,165,99,53,196,9,194,27,142,120,33,35,150,226,49,149,77,206,162,88,87,38,1,52,0,254,3,200,199,32,153,104,39,32,78,218,91,144,24,17,147,178,76,8,208,47,44,134,76,214,29,238,240,240,35,160,148,119,252,16,205,114,211,66,59,237,34,159,98,176,249,28,26,54,209,103,240,84,156,89,120,61,38,205,112,124,101,78,13,35,228,61,87,190,245,3,235,157,159,144,241,88,165,254,227,199,18,53,135,205,88,177,130,57,6,172,104,9,27,146,245,4,191,161,7,1,209,109,40,243,55,122,14,19,148,89,250,123,61,135,249,136,198,121,176,75,125,192,115,174,48,216,124,14,109,211,115,222,199,123,207,115,116,152,82,193,78,79,217,138,7,184,203,62,22,211,146,157,56,15,249,162,113,139,41,117,186,196,7,218,231,211,114,203,43,238,151,234,14,200,237,160,100,215,20,26,90,255,105,176,8,13,223,205,18,104,11,59,2,189,141,194,219,129,124,8,216,228,116,221,174,201,215,134,116,91,250,55,114,37,112,101,133,191,219,55,74,198,29,240,231,186,96,175,180,55,28,210,223,216,170,96,230,206,207,180,157,197,57,182,241,238,114,51,131,211,170,190,178,40,24,111,150,210,152,254,90,121,89,85,91,34,75,170,5,71,254,32,57,116,164,153,133,42,148,246,108,143,254,137,119,160,12,189,198,206,199,153,40,96,157,85,232,34,237,242,250,28,69,157,151,93,243,204,166,8,56,155,7,107,137,198,18,217,65,81,19,41,106,185,109,20,86,213,82,196,108,75,229,126,205,104,178,168,44,219,218,152,219,235,54,139,177,121,72,115,37,91,60,64,227,88,128,124,184,242,249,227,84,5,151,48,63,143,46,26,51,67,57,28,170,254,74,97,255,205,220,47,15,244,250,249,203,178,136,228,196,202,244,175,23,198,46,59,11,166,36,245,124,89,144,149,20,231,128,183,70,209,183,137,241,205,214,217,34,44,217,73,159,2,104,203,133,0,140,95,101,35,158,20,203,213,100,132,46,239,248,235,138,93,10,43,244,232,236,182,188,100,38,141,215,76,91,212,47,146,201,65,166,238,78,250,44,124,35,61,73,129,27,69,162,179,183,151,58,209,95,252,135,220,93,204,254,227,41,7,95,34,181,213,180,37,41,57,176,242,105,181,114,220,158,81,173,165,80,155,117,104,131,157,33,245,204,110,129,209,156,239,132,141,96,99,252,250,46,240,57,85,179,40,115,117,106,155,132,32,239,238,93,19,84,3,51,126,221,15,140,238,12,183,14,205,123,101,51,6,107,23,113,121,165,107,173,3,213,94,161,42,175,208,142,222,106,83,175,149,221,66,65,217,187,233,33,68,51,218,169,131,242,192,95,100,103,166,74,78,39,174,92,0,220,72,86,202,228,64,204,214,238,18,149,92,247,202,151,82,248,135,87,146,92,150,229,130,245,196,232,170,69,112,64,122,162,203,47,176,109,235,201,89,13,58,81,145,17,23,23,208,166,229,98,188,221,241,99,224,223,41,36,179,215,236,106,239,231,212,249,121,121,213,85,210,151,90,41,176,23,93,111,34,58,189,1,123,149,36,123,255,245,203,111,189,222,192,209,186,194,117,166,40,253,47,191,75,123,3,83,235,10,215,153,162,244,191,250,134,238,13,60,45,11,92,101,137,145,255,210,107,191,55,240,195,168,95,101,134,209,254,226,139,196,55,176,51,211,191,202,16,163,62,210,210,108,121,179,134,94,132,26,236,142,123,186,20,157,46,56,201,93,11,87,221,110,222,199,207,205,54,240,149,123,5,78,55,236,165,251,32,48,231,31,96,31,118,140,235,223,202,236,143,95,238,234,225,116,247,59,247,170,64,136,78,19,147,148,127,110,135,164,111,5,8,111,228,144,11,56,126,75,117,130,135,241,127,50,84,163,80,145,48,0,0 };