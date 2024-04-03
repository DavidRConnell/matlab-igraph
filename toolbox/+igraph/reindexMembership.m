function memb = reindexMembership(memb)
    arguments
        memb {mustBePartition};
    end

    memb = mexIgraphDispatcher(mfilename(), memb);
end
