function opts = parseOptionsAtlas(opts)
    arguments
        opts.atlasId (1, 1) {mustBeNonnegative, mustBeInteger, ...
                             mustBeLessThan(opts.atlasId,1254)} = 2;
    end
    opts.atlasId = opts.atlasId - 1;
end
