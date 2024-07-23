function memb = reindexMembership(memb)
%REINDEXMEMBERSHIP renumber memberships communities to be sequential
%   MEMB = REINDEXMEMBERSHIP(MEMB) For a membership MEMB with n unique
%   communities, return an equivalent membership vector with communities IDs
%   between 1--n.
%
%   See also IGRAPH.CLUSTER, IGRAPH.MODULARITY.

    arguments
        memb {igraph.args.mustBePartition};
    end

    memb = mexIgraphDispatcher(mfilename(), memb);
end
