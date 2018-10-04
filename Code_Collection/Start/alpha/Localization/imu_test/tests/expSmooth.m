function [ output ] = expSmooth( data, alpha )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

    output = zeros;
    output(1) = data(1);
        
    for i = 2 : length(data)
        residue = output(i-1) - data(i);
        output(i) = output(i-1) + alpha * residue;
    end

end

