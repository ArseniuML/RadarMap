clear all
close all

im = double(imread('temp.tif'));
im = im(2, :);
[temp size] = size(im);

figure
plot(1:1:size, im);
axis([1 size min(im) max(im)]);
grid on