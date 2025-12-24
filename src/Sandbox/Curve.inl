#ifndef CURVE_INL_DEFINED
#define CURVE_INL_DEFINED

template<typename Func>
void Curve::CalculateCurve(float xmin, float xmax, int n, Func&& f)
{
    m_vertices.clear();
    m_vertices.reserve(n);

    float step = (xmax - xmin) / (n - 1);

    for (int i = 0; i < n; ++i)
    {
        float x = xmin + i * step;
        m_vertices.push_back({ origin.x + x, origin.y + f(x) });
    }
}

template<typename Func>
void Curve::CalculateShape(Func&& f)
{
    m_vertices.clear();

    m_vertices = f();
}

#endif