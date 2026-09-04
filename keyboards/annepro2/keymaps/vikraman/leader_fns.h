#pragma once

// wraps cursor in a 2-char pair, e.g. "()"
void leader_wrap(void *data);

// wraps cursor in \begin{name}/\end{name}
void leader_tex_env(void *data);
