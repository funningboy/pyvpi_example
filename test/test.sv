
interface pyAESECBenc(
    input reg [127:0] key,
    input reg [255:0] text,
    output reg [255:0] enc
    );

    always@(text, key) begin
        if (text && key) begin
            $pyvpi_main("aes_ecb_enc.py");
        end
    end
endinterface

interface pyAESECBdec(
    input reg [127:0] key,
    input reg [255:0] dec,
    output reg [255:0] text
    );

    always@(dec, key) begin
        if (dec && key) begin
            $pyvpi_main("aes_ecb_dec.py");
        end
    end
endinterface

interface pyadaptor(
    input reg [31:0]  a,
    input reg [31:0]  b,
    output reg [31:0]  c
    );

    always@(a, b) begin
        $pyvpi_main("tt.py");
    end
endinterface



module top;

    reg [31:0]    a;
    reg [31:0]    b;
    reg [31:0]    c;

    initial begin
        $monitor("sv %t, a:%0d + b:%0d = c:%0d", $time, a, b, c);
    end

    pyadaptor u_pyadaptor(
        .a(a),
        .b(b),
        .c(c)
    );

    initial begin
        a = 0;
        b = 0;
        #10;
        a = 1;
        b = 2;
        #10;
        a = 2;
        b = 3;
    end

    reg [127:0] key;
    reg [255:0] text;
    reg [255:0] enc;
    reg [255:0] dec;

    pyAESECBenc u_pyAESECBenc(
        .key(key),
        .text(text),
        .enc(enc)
    );

    pyAESECBdec u_pyAESECBdec(
        .key(key),
        .dec(enc),
        .text(dec)
    );


    initial begin
        $monitor("sv %t, key:%h + text:%h = enc:%h, dec:%h", $time, key, text, enc, dec);
    end

    initial begin
        key = 'h0;
        text = 'h0;
        #10;
        key = 128'h2b7e151628aed2a6abf7158809cf4f3c;
        text = 256'h6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51;
    end

   initial begin
    #100;
    $finish;
   end

endmodule
