<?php

include("lib/assembly-Library.inc"); 

$time_start = microtime(true);



$pilha = pack("C*", 
0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x00 ,0x00 ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0x80 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x00 ,0x00
);


$memoria = pack("C*", 
0x01 ,0x23 ,0x45 ,0x67 ,0x89 ,0xAB ,0xCD ,0xEF ,0xFE ,0xDC ,0xBA ,0x98 ,0x76 ,0x54 ,0x32 ,0x10
);                                 


$senha = "admin";

$pilha[0x40] = chr(8 * strlen($senha)); // campo 0x44 indica o tamanho de bytes da string digitada que no maximo por ter 8 caracters = 0x40

$senha .= chr(0x80).str_repeat(chr(0),8-strlen($senha));

for($i=0; $i < strlen($senha); $i++)
 $pilha[8+$i] = $senha[$i];
 
 

$asm = new assembly();


/*** Inicializa *****/          
          
$asm->MOV("EAX", 0x0019F4D8); // inicializa
$asm->MOV("ECX", 0x0019F4A8); // inicializa
$asm->MOV("ECX", 0x00000000); // inicializa
$asm->MOV("EBX", 0x00000008); // inicializa
$asm->MOV("ESP", 0x0019F484);  // PILHA
$asm->MOV("EBP", 0x0019F510); // inicializa
$asm->MOV("ESI", 0x00000008); // inicializa


$asm->MOV("EDI", 0x0019F4C0); // inicializa
$asm->PUSH_MEMORY($memoria);

$asm->PUSH_STACK($pilha); // inserção de dados na pilha

/*** FIM Inicializa ****/



$asm->PUSH("EBX");
$asm->MOV("EBX" , "DWORD PTR DS:[EDI+8]"); 

//$asm->show_memory();
//$asm->show_registers();
//die("");

$asm->PUSH("EBP");
$asm->MOV("EBP" , "DWORD PTR DS:[EDI+0C]");
$asm->PUSH("ESI");
$asm->MOV("ESI" , "DWORD PTR DS:[EDI+4]");
$asm->LEA("ECX" , "[LOCAL.15]");


$asm->MOV("EAX" , "DWORD PTR DS:[EDI]");
$asm->MOV("EDX" , "EBX");
$asm->BITAND("EDX" , "ESI");
$asm->MOV("ECX" , "ESI");
$asm->NOT("ECX");
$asm->BITAND("ECX" , "EBP");
$asm->BITOR("ECX" , "EDX");
$asm->ADD("ECX" , "EAX");
$asm->MOV("EAX" , "DWORD PTR SS:[ESP+14]"); 
$asm->LEA("EAX" , "[EAX+ECX+D76AA478]");     
$asm->ROL("EAX" , 0x7 );
$asm->ADD("EAX" , "ESI");
$asm->MOV("ECX" , "EAX");
$asm->NOT("ECX");          
$asm->BITAND("ECX" , "EBX");
$asm->MOV("EDX" , "ESI");
$asm->BITAND("EDX" , "EAX");
$asm->BITOR("ECX" , "EDX"); 
$asm->ADD("ECX" , "DWORD PTR SS:[LOCAL.14]");
$asm->LEA("ECX" , "[EBP+ECX+E8C7B756]");
$asm->ROL("ECX" , 0x0C );
$asm->ADD("ECX" , "EAX");
$asm->MOV("EDX" , "ECX");
$asm->NOT("EDX");
$asm->BITAND("EDX" , "ESI");
$asm->MOV("EBP" , "ECX");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EDX" , "EBP");
$asm->ADD("EDX" , "DWORD PTR SS:[LOCAL.13]");
$asm->MOV("EBP" , "ECX");
$asm->LEA("EDX" , "[EBX+EDX+242070DB]");
$asm->ROR("EDX" , 0x0F );
$asm->ADD("EDX" , "ECX");
$asm->BITAND("EBP" , "EDX");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.12]");
$asm->MOV("EBP" , "EDX");
$asm->LEA("ESI" , "[ESI+EBX+C1BDCEEE]");
$asm->ROR("ESI" , 0x0A );
$asm->ADD("ESI" , "EDX");
$asm->BITAND("EBP" , "ESI");
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ECX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.11]");
$asm->MOV("EBP" , "ESI");
$asm->LEA("EAX" , "[EAX+EBX+F57C0FAF]");
$asm->ROL("EAX" , 0x7 );
$asm->ADD("EAX" , "ESI");
$asm->BITAND("EBP" , "EAX");
$asm->MOV("EBX" , "EAX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EDX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.10]");
$asm->LEA("ECX" , "[ECX+EBX+4787C62A]");
$asm->ROL("ECX" , 0x0C );
$asm->ADD("ECX" , "EAX");
$asm->MOV("EBX" , "ECX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ESI");
$asm->MOV("EBP" , "ECX");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.9]");
$asm->LEA("EDX" , "[EDX+EBX+A8304613]");
$asm->ROR("EDX" , 0x0F );
$asm->ADD("EDX" , "ECX");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EAX");
$asm->MOV("EBP" , "ECX");
$asm->BITAND("EBP" , "EDX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.8]");
$asm->MOV("EBP" , "EDX");
$asm->LEA("ESI" , "[ESI+EBX+FD469501]");
$asm->ROR("ESI" , 0x0A );
$asm->ADD("ESI" , "EDX");
$asm->BITAND("EBP" , "ESI");
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ECX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.7]");
$asm->MOV("EBP" , "ESI");
$asm->LEA("EAX" , "[EAX+EBX+698098D8]");
$asm->ROL("EAX" , 0x7 );
$asm->ADD("EAX" , "ESI");
$asm->BITAND("EBP" , "EAX");
$asm->MOV("EBX" , "EAX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EDX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.6]");
$asm->LEA("ECX" , "[ECX+EBX+8B44F7AF]");
$asm->ROL("ECX" , 0x0C );
$asm->ADD("ECX" , "EAX");
$asm->MOV("EBX" , "ECX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ESI");
$asm->MOV("EBP" , "ECX");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.5]");
$asm->MOV("EBP" , "ECX");
$asm->LEA("EDX" , "[EDX+EBX+FFFF5BB1]");
$asm->ROR("EDX" , 0x0F );
$asm->ADD("EDX" , "ECX");
$asm->BITAND("EBP" , "EDX");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.4]");
$asm->MOV("EBP" , "EDX");
$asm->LEA("ESI" , "[ESI+EBX+895CD7BE]");
$asm->ROR("ESI" , 0x0A );
$asm->ADD("ESI" , "EDX");
$asm->BITAND("EBP" , "ESI");
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ECX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.3]");
$asm->MOV("EBP" , "ESI");
$asm->LEA("EAX" , "[EAX+EBX+6B901122]");
$asm->ROL("EAX" , 0x7 );
$asm->ADD("EAX" , "ESI");
$asm->BITAND("EBP" , "EAX");
$asm->MOV("EBX" , "EAX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EDX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.2]");
$asm->LEA("ECX" , "[ECX+EBX+FD987193]");
$asm->ROL("ECX" , 0x0C );
$asm->ADD("ECX" , "EAX");
$asm->MOV("EBX" , "ECX");
$asm->NOT("EBX");
$asm->MOV("DWORD PTR SS:[LOCAL.17]" , "EBX");
$asm->BITAND("EBX" , "ESI");
$asm->MOV("EBP" , "ECX");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.1]");
$asm->LEA("EDX" , "[EDX+EBX+A679438E]");
$asm->ROR("EDX" , 0x0F );
$asm->ADD("EDX" , "ECX");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->MOV("DWORD PTR SS:[LOCAL.16]" , "EBX");

$asm->BITAND("EBX" , "EAX");
$asm->MOV("EBP" , "ECX");
$asm->BITAND("EBP" , "EDX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.0]");
$asm->MOV("EBP" , "ECX");
$asm->LEA("ESI" , "[ESI+EBX+49B40821]");
$asm->MOV("EBX" , "DWORD PTR SS:[LOCAL.17]");
$asm->BITAND("EBX" , "EDX");
$asm->ROR("ESI" , 0x0A );
$asm->ADD("ESI" , "EDX");
$asm->BITAND("EBP" , "ESI");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.14]");
$asm->MOV("EBP" , "EDX");
$asm->LEA("EAX" , "[EAX+EBX+F61E2562]");
$asm->MOV("EBX" , "DWORD PTR SS:[LOCAL.16]");
$asm->BITAND("EBX" , "ESI");
$asm->ROL("EAX" , 0x5 );
$asm->ADD("EAX" , "ESI");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EBX" , "EBP");


$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.9]");
$asm->LEA("ECX" , "[ECX+EBX+C040B340]");
$asm->ROL("ECX" , 0x9 );
$asm->ADD("ECX" , "EAX");
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EAX");
$asm->MOV("EBP" , "ECX");
$asm->BITAND("EBP" , "ESI");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.4]");
$asm->LEA("EDX" , "[EDX+EBX+265E5A51]");
$asm->ROL("EDX" , 0x0E );
$asm->ADD("EDX" , "ECX");
$asm->MOV("EBX" , "EAX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ECX");
$asm->MOV("EBP" , "EDX");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.15]");
$asm->MOV("EBP" , "ECX");
$asm->LEA("ESI" , "[ESI+EBX+E9B6C7AA]");
$asm->ROR("ESI" , 0x0C );
$asm->ADD("ESI" , "EDX");
$asm->BITAND("EBP" , "ESI");
$asm->MOV("EBX" , "ECX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EDX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.10]");
$asm->MOV("EBP" , "EDX");
$asm->LEA("EAX" , "[EAX+EBX+D62F105D]");
$asm->ROL("EAX" , 0x5 );
$asm->ADD("EAX" , "ESI");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ESI");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.5]");
$asm->LEA("ECX" , "[ECX+EBX+2441453]");
$asm->ROL("ECX" , 0x9 );
$asm->ADD("ECX" , "EAX");
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EAX");
$asm->MOV("EBP" , "ECX");
$asm->BITAND("EBP" , "ESI");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.0]");
$asm->LEA("EDX" , "[EDX+EBX+D8A1E681]");
$asm->ROL("EDX" , 0x0E );
$asm->MOV("EBX" , "EAX");
$asm->NOT("EBX");
$asm->ADD("EDX" , "ECX");
$asm->BITAND("EBX" , "ECX");
$asm->MOV("EBP" , "EDX");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.11]");
$asm->MOV("EBP" , "ECX");
$asm->LEA("ESI" , "[ESI+EBX+E7D3FBC8]");
$asm->ROR("ESI" , 0x0C );
$asm->ADD("ESI" , "EDX");
$asm->BITAND("EBP" , "ESI");
$asm->MOV("EBX" , "ECX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EDX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.6]");
$asm->MOV("EBP" , "EDX");
$asm->LEA("EAX" , "[EAX+EBX+21E1CDE6]");
$asm->ROL("EAX" , 0x5 );
$asm->ADD("EAX" , "ESI");
$asm->BITAND("EBP" , "EAX");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ESI");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.1]");
$asm->LEA("ECX" , "[ECX+EBX+C33707D6]");
$asm->ROL("ECX" , 0x9 );
$asm->ADD("ECX" , "EAX");
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EAX");
$asm->MOV("EBP" , "ECX");
$asm->BITAND("EBP" , "ESI");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.12]");
$asm->LEA("EDX" , "[EDX+EBX+F4D50D87]");
$asm->ROL("EDX" , 0x0E );
$asm->ADD("EDX" , "ECX");
$asm->MOV("EBX" , "EAX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ECX");
$asm->MOV("EBP" , "EDX");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.7]");
$asm->MOV("EBP" , "ECX");
$asm->LEA("ESI" , "[ESI+EBX+455A14ED]");
$asm->ROR("ESI" , 0x0C );
$asm->ADD("ESI" , "EDX");
$asm->BITAND("EBP" , "ESI");
$asm->MOV("EBX" , "ECX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EDX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.2]");
$asm->MOV("EBP" , "EDX");
$asm->LEA("EAX" , "[EAX+EBX+A9E3E905]");
$asm->ROL("EAX" , 0x5 );
$asm->ADD("EAX" , "ESI");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ESI");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.13]");
$asm->LEA("ECX" , "[ECX+EBX+FCEFA3F8]");
$asm->ROL("ECX" , 0x9 );
$asm->ADD("ECX" , "EAX");
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "EAX");
$asm->MOV("EBP" , "ECX");
$asm->BITAND("EBP" , "ESI");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.8]");
$asm->LEA("EDX" , "[EDX+EBX+676F02D9]");
$asm->ROL("EDX" , 0x0E );
$asm->MOV("EBX" , "EAX");
$asm->ADD("EDX" , "ECX");
$asm->NOT("EBX");
$asm->BITAND("EBX" , "ECX");
$asm->MOV("EBP" , "EDX");
$asm->BITAND("EBP" , "EAX");
$asm->BITOR("EBX" , "EBP");
$asm->ADD("EBX" , "DWORD PTR SS:[ESP+44]");
$asm->LEA("ESI" , "[ESI+EBX+8D2A4C8A]");
$asm->ROR("ESI" , 0x0C );
$asm->ADD("ESI" , "EDX");
$asm->MOV("EBX" , "ECX");
$asm->BITXOR("EBX" , "EDX");
$asm->BITXOR("EBX" , "ESI");
$asm->ADD("EBX" , "DWORD PTR SS:[ESP+28]");
$asm->LEA("EAX" , "[EAX+EBX+FFFA3942]");
$asm->ROL("EAX" , 0x4 );
$asm->ADD("EAX" , "ESI");
$asm->MOV("EBX" , "EDX");
$asm->BITXOR("EBX" , "ESI");
$asm->BITXOR("EBX" , "EAX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.7]");
$asm->LEA("EBX" , "[ECX+EBX+8771F681]");
$asm->ROL("EBX" , 0x0B );
$asm->ADD("EBX" , "EAX");
$asm->MOV("ECX" , "EBX");
$asm->BITXOR("ECX" , "ESI");
$asm->BITXOR("ECX" , "EAX");
$asm->ADD("ECX" , "DWORD PTR SS:[LOCAL.4]");
$asm->MOV("EBP" , "EBX");
$asm->LEA("EDX" , "[EDX+ECX+6D9D6122]");
$asm->ROL("EDX" , 0x10 );
$asm->ADD("EDX" , "EBX");
$asm->BITXOR("EBP" , "EDX");
$asm->MOV("ECX" , "EBP");
$asm->BITXOR("ECX" , "EAX");
$asm->ADD("ECX" , "DWORD PTR SS:[LOCAL.1]");
$asm->LEA("ECX" , "[ESI+ECX+FDE5380C]");
$asm->ROR("ECX" , 0x9 );
$asm->ADD("ECX" , "EDX");
$asm->BITXOR("EBP" , "ECX");
$asm->ADD("EBP" , "DWORD PTR SS:[LOCAL.14]");
$asm->MOV("ESI" , "EDX");
$asm->BITXOR("ESI" , "ECX");
$asm->LEA("EAX" , "[EBP+EAX+A4BEEA44]");
$asm->ROL("EAX" , 0x4 );
$asm->ADD("EAX" , "ECX");
$asm->BITXOR("ESI" , "EAX");
$asm->ADD("ESI" , "DWORD PTR SS:[LOCAL.11]");
$asm->LEA("ESI" , "[EBX+ESI+4BDECFA9]");
$asm->ROL("ESI" , 0x0B );
$asm->ADD("ESI" , "EAX");
$asm->MOV("EBX" , "ESI");
$asm->BITXOR("EBX" , "ECX");
$asm->BITXOR("EBX" , "EAX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.8]");
$asm->LEA("EDX" , "[EDX+EBX+F6BB4B60]");
$asm->ROL("EDX" , 0x10 );
$asm->ADD("EDX" , "ESI");
$asm->MOV("EBX" , "ESI");
$asm->BITXOR("EBX" , "EDX");
$asm->MOV("EBP" , "EBX");
$asm->BITXOR("EBP" , "EAX");
$asm->ADD("EBP" , "DWORD PTR SS:[LOCAL.5]");
$asm->LEA("ECX" , "[EBP+ECX+BEBFBC70]");
$asm->ROR("ECX" , 0x9 );
$asm->ADD("ECX" , "EDX");
$asm->BITXOR("EBX" , "ECX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.2]");
$asm->LEA("EAX" , "[EAX+EBX+289B7EC6]");
$asm->ROL("EAX" , 0x4 );
$asm->ADD("EAX" , "ECX");
$asm->MOV("EBX" , "EDX");
$asm->BITXOR("EBX" , "ECX");
$asm->BITXOR("EBX" , "EAX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.15]");
$asm->LEA("ESI" , "[ESI+EBX+EAA127FA]");
$asm->ROL("ESI" , 0x0B );
$asm->ADD("ESI" , "EAX");
$asm->MOV("EBX" , "ESI");
$asm->BITXOR("EBX" , "ECX");
$asm->BITXOR("EBX" , "EAX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.12]");
$asm->LEA("EBX" , "[EDX+EBX+D4EF3085]");
$asm->ROL("EBX" , 0x10 );
$asm->ADD("EBX" , "ESI");
$asm->MOV("EDX" , "ESI");
$asm->BITXOR("EDX" , "EBX");
$asm->MOV("EBP" , "EDX");
$asm->BITXOR("EBP" , "EAX");
$asm->ADD("EBP" , "DWORD PTR SS:[LOCAL.9]");
$asm->LEA("ECX" , "[EBP+ECX+4881D05]");
$asm->ROR("ECX" , 0x9 );
$asm->ADD("ECX" , "EBX");
$asm->BITXOR("EDX" , "ECX");
$asm->ADD("EDX" , "DWORD PTR SS:[LOCAL.6]");
$asm->LEA("EAX" , "[EAX+EDX+D9D4D039]");
$asm->MOV("EDX" , "EBX");
$asm->BITXOR("EDX" , "ECX");
$asm->ROL("EAX" , 0x4 );
$asm->ADD("EAX" , "ECX");
$asm->BITXOR("EDX" , "EAX");
$asm->ADD("EDX" , "DWORD PTR SS:[LOCAL.3]");
$asm->LEA("EDX" , "[ESI+EDX+E6DB99E5]");
$asm->ROL("EDX" , 0x0B );
$asm->ADD("EDX" , "EAX");
$asm->MOV("ESI" , "EDX");
$asm->BITXOR("ESI" , "ECX");
$asm->BITXOR("ESI" , "EAX");
$asm->ADD("ESI" , "DWORD PTR SS:[LOCAL.0]");
$asm->LEA("ESI" , "[EBX+ESI+1FA27CF8]");
$asm->ROL("ESI" , 0x10 );
$asm->ADD("ESI" , "EDX");
$asm->MOV("EBX" , "EDX");
$asm->BITXOR("EBX" , "ESI");
$asm->BITXOR("EBX" , "EAX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.13]");
$asm->LEA("ECX" , "[ECX+EBX+C4AC5665]");
$asm->ROR("ECX" , 0x9 );
$asm->ADD("ECX" , "ESI");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "ECX");
$asm->BITXOR("EBX" , "ESI");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.15]");
$asm->LEA("EAX" , "[EAX+EBX+F4292244]");
$asm->ROL("EAX" , 0x6 );
$asm->ADD("EAX" , "ECX");
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "EAX");
$asm->BITXOR("EBX" , "ECX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.8]");
$asm->LEA("EDX" , "[EDX+EBX+432AFF97]");
$asm->ROL("EDX" , 0x0A );
$asm->ADD("EDX" , "EAX");
$asm->MOV("EBX" , "ECX");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "EDX");
$asm->BITXOR("EBX" , "EAX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.1]");
$asm->LEA("ESI" , "[ESI+EBX+AB9423A7]");
$asm->ROL("ESI" , 0x0F );
$asm->ADD("ESI" , "EDX");
$asm->MOV("EBX" , "EAX");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "ESI");
$asm->BITXOR("EBX" , "EDX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.10]");
$asm->LEA("ECX" , "[ECX+EBX+FC93A039]");
$asm->ROR("ECX" , 0x0B );
$asm->ADD("ECX" , "ESI");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "ECX");
$asm->BITXOR("EBX" , "ESI");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.3]");
$asm->LEA("EAX" , "[EAX+EBX+655B59C3]");
$asm->ROL("EAX" , 0x6 );
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->ADD("EAX" , "ECX");
$asm->BITOR("EBX" , "EAX");
$asm->BITXOR("EBX" , "ECX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.12]");
$asm->LEA("EDX" , "[EDX+EBX+8F0CCC92]");
$asm->ROL("EDX" , 0x0A );
$asm->ADD("EDX" , "EAX");
$asm->MOV("EBX" , "ECX");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "EDX");
$asm->BITXOR("EBX" , "EAX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.5]");
$asm->LEA("ESI" , "[ESI+EBX+FFEFF47D]");
$asm->ROL("ESI" , 0x0F );
$asm->ADD("ESI" , "EDX");
$asm->MOV("EBX" , "EAX");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "ESI");
$asm->BITXOR("EBX" , "EDX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.14]");
$asm->LEA("ECX" , "[ECX+EBX+85845DD1]");
$asm->ROR("ECX" , 0x0B );
$asm->ADD("ECX" , "ESI");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "ECX");
$asm->BITXOR("EBX" , "ESI");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.7]");
$asm->LEA("EAX" , "[EAX+EBX+6FA87E4F]");
$asm->ROL("EAX" , 0x6 );
$asm->ADD("EAX" , "ECX");
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "EAX");
$asm->BITXOR("EBX" , "ECX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.0]");
$asm->LEA("EDX" , "[EDX+EBX+FE2CE6E0]");
$asm->ROL("EDX" , 0x0A );
$asm->ADD("EDX" , "EAX");
$asm->MOV("EBX" , "ECX");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "EDX");
$asm->BITXOR("EBX" , "EAX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.9]");
$asm->LEA("ESI" , "[ESI+EBX+A3014314]");
$asm->MOV("EBX" , "EAX");
$asm->NOT("EBX");
$asm->ROL("ESI" , 0x0F );
$asm->ADD("ESI" , "EDX");
$asm->BITOR("EBX" , "ESI");
$asm->BITXOR("EBX" , "EDX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.2]");
$asm->LEA("ECX" , "[ECX+EBX+4E0811A1]");
$asm->ROR("ECX" , 0x0B );
$asm->ADD("ECX" , "ESI");
$asm->MOV("EBX" , "EDX");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "ECX");
$asm->BITXOR("EBX" , "ESI");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.11]");
$asm->LEA("EAX" , "[EAX+EBX+F7537E82]");
$asm->ROL("EAX" , 0x6 );
$asm->ADD("EAX" , "ECX");
$asm->MOV("EBX" , "ESI");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "EAX");
$asm->BITXOR("EBX" , "ECX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.4]");
$asm->LEA("EDX" , "[EDX+EBX+BD3AF235]");
$asm->ROL("EDX" , 0x0A );
$asm->ADD("EDX" , "EAX");
$asm->MOV("EBX" , "ECX");
$asm->NOT("EBX");
$asm->BITOR("EBX" , "EDX");
$asm->BITXOR("EBX" , "EAX");
$asm->ADD("EBX" , "DWORD PTR SS:[LOCAL.13]");
$asm->LEA("ESI" , "[ESI+EBX+2AD7D2BB]");
$asm->MOV("EBX" , "DWORD PTR DS:[EDI]");
$asm->ADD("EBX" , "EAX");
$asm->ROL("ESI" , 0x0F );
$asm->NOT("EAX");
$asm->ADD("ESI" , "EDX");
$asm->BITOR("EAX" , "ESI");
$asm->BITXOR("EAX" , "EDX");
$asm->ADD("EAX" , "DWORD PTR SS:[LOCAL.6]");
echo "EBX = ".base_convert( $asm->registers["EBX"] , 10, 16)."\r\n";
$asm->MOV("DWORD PTR DS:[EDI]" , "EBX");
$asm->LEA("EAX" , "[ECX+EAX+EB86D391]");
$asm->ROR("EAX" , 0x0B );
$asm->ADD("EAX" , "DWORD PTR DS:[EDI+4]");
$asm->ADD("EAX" , "ESI");
echo "EAX = ".base_convert( $asm->registers["EAX"] , 10, 16)."\r\n";
$asm->MOV("DWORD PTR DS:[EDI+4]" , "EAX");
$asm->MOV("EAX" , "DWORD PTR DS:[EDI+8]");
$asm->ADD("EAX" , "ESI");
echo "EAX = ".base_convert( $asm->registers["EAX"] , 10, 16)."\r\n";
$asm->MOV("DWORD PTR DS:[EDI+8]" , "EAX");
$asm->MOV("EAX" , "DWORD PTR DS:[EDI+0C]");
$asm->POP("ESI");
$asm->ADD("EAX" , "EDX");
$asm->POP("EBP");
echo "EAX = ".base_convert( $asm->registers["EAX"] , 10, 16)."\r\n";
$asm->MOV("DWORD PTR DS:[EDI+0C]" , "EAX");
$asm->POP("EBX");


//$asm->show_stack();
//$asm->show_registers(); 
$asm->show_memory();
//die("");




/************ Processo final após realizar a geração do MD5  ***************/

$mem = $asm->memory;


$degug = 0;

$EDI = $asm->get_register("EDI"); 


$ret = "";

$debug = 0;

for($EDI=$asm->get_register("EDI"); $EDI <= $asm->get_register("EDI") + 14; $EDI+=2)
{

if ($debug) echo " \$asm->memory[".str_pad( base_convert($EDI,10,16), 8, "0", STR_PAD_LEFT )."] = ". $asm->memory[$EDI]." \r\n";

$EAX = $mem[$EDI] + $mem[$EDI+1];
$EBX = base_convert("3E", 16,10);


$DL = ($EAX % $EBX); $EAX = (int)($EAX / $EBX);  // IDIV EBX

$str[$ECX-1] = $DL; //  MOV BYTE PTR DS:[ECX-1],DL

IF($DL <= 9)
{
	$DL = $DL + base_convert("30", 16,10);
}
ELSE IF (($DL >=  base_convert("0A", 16,10) ) && ($DL <=  base_convert("23", 16,10) ))
{
	$DL = $DL + base_convert("37", 16,10);
}
ELSE
{
	$DL = $DL + base_convert("3D", 16,10);
}

if ($debug) echo $EAX." -- ".$DL." -- ".chr($DL)."\r\n";

$ret .= chr($DL);

$str[$ECX-1] = $DL; //  MOV BYTE PTR DS:[ECX-1],DL

}

$time_end = microtime(true);
$time = $time_end - $time_start;    
echo "Did nothing in $time seconds\n";

die("\r\n\r\nCriptografado: {$ret}\r\n");


?>