function opts = parseOptionsPrufer(opts)
    arguments
        opts.prufer {mustBeInteger, mustBeVector, mustBePositive} = 1:10;
    end

    opts.prufer = opts.prufer - 1;
end
