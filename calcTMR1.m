% calcTMR1.m
% Calcula TMR1H y TMR1L para PIC16F887
%
% Uso:
%   [TMR1H, TMR1L] = calcTMR1(fclk, Tdelay, prescaler)
%
% Ejemplo:
%   [H, L] = calcTMR1(4e6, 1, 64) % 4 MHz, 1 s, prescaler=64

function [TMR1H, TMR1L] = calcTMR1(fclk, Tdelay, prescaler)
    % fclk      : frecuencia del oscilador en Hz (p.ej. 4e6)
    % Tdelay    : retardo deseado en segundos (p.ej. 1 para 1 s)
    % prescaler : valor del prescaler (1, 2, 4, 8, 16, 64, 256)

    % Tamaño del timer 16 bits
    Size = 2^16;

    % Tiempo de instrucción
    t_instr = 4 / fclk;      % en segundos

    % N ticks para el retardo deseado
    ticks = Tdelay / (prescaler * t_instr);

    % Valor inicial de carga
    TMR1_init = round(Size - ticks);

    % Asegurarse de que está en rango [0, 65535]
    if TMR1_init < 0 || TMR1_init > 65535
        error('Con estos parámetros el valor inicial (%d) está fuera de rango.', TMR1_init);
    end

    % Descomponer en alto y bajo
    TMR1H = bitshift(TMR1_init, -8);        % byte alto
    TMR1L = bitand(TMR1_init, 255);         % byte bajo

    % Mostrar en hexadecimal
    fprintf('TMR1_init = %d (0x%04X)\n', TMR1_init, TMR1_init);
    fprintf('TMR1H = %d (0x%02X)\n', TMR1H, TMR1H);
    fprintf('TMR1L = %d (0x%02X)\n', TMR1L, TMR1L);
end
