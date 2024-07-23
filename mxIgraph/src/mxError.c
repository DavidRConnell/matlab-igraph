/* Copyright 2024 David R. Connell <david32@dcon.addy.io>.
 *
 * This file is part of matlab-igraph.
 *
 * matlab-igraph is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * matlab-igraph is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with matlab-igraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include <mxIgraph.h>

void mxIgraphErrorNotImplemented(const char* caller, const char* method)
{
  mexErrMsgIdAndTxt("igraph:notImplemented",
                    "%s method %s has not been implemented.\n\n%s", caller, method,
                    "has not been implemented yet.",
                    "Please report an issue or submit a pull request to github.");
}

void mxIgraphErrorUnknownMethod(const char* caller, const char* method)
{
  mexErrMsgIdAndTxt("igraph:internal:unknownMethod",
                    "Unknown %s method %s.\n\n%s",
                    caller, method, "Please report an issue to github.");
}
