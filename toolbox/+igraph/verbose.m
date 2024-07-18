function verbose(handler, ison)
%VERBOSE turn data printing on and off
%   VERBOSE(HANDLER, TF) turn a handler's printing on or off. Handler can be
%       one of 'warning', 'progress', or 'status'. If TF, igraph will use the
%       handler; if set to false, igraph will not print messages with the
%       provided handler.

    arguments
        handler (1, :) char ...
            {igraph.args.mustBeMemberi(handler, ...
                                       {'warning', 'progress', 'status'})};
        ison (1, 1) logical;
    end

    mexIgraphDispatcher(mfilename(), handler, ison);
end
