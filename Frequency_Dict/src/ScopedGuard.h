#pragma once

// Used to avoid leaking resources. Yes, they anyway will be freed/closed when
// the program is terminated, but let's follow the good practisice.
template<typename F>
struct ScopedGuard
{
	F destuctor;
	bool is_active;

	explicit ScopedGuard(const F &func) :
		destuctor(func), is_active(true) {}

	~ScopedGuard() { if (is_active) destuctor(); }

	ScopedGuard(const ScopedGuard&) = delete;
	ScopedGuard(ScopedGuard &&) = delete;
	ScopedGuard& operator=(const ScopedGuard&) = delete;
	ScopedGuard& operator=(ScopedGuard&&) = delete;
};

template <typename F>
inline ScopedGuard<F>
create_scoped_guard(F func)
{
	return ScopedGuard<F>(func);
}
