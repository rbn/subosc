#include "scope.h"
#include "raylib.h"

void scope_capture_init(ScopeCapture *capture)
{
    capture->count = 0;
    atomic_init(&capture->ready, 0);
}

void scope_capture_push(ScopeCapture *capture, const float *samples, int num_frames)
{
    if (atomic_load(&capture->ready) != 0)
    {
        return;
    }

    for (int n = 0; n < num_frames; n++)
    {
        capture->samples[capture->count++] = samples[n];

        if (capture->count == SCOPE_CAPTURE_SAMPLES)
        {
            atomic_store(&capture->ready, 1);
            break;
        }
    }
}

void scope_display_update(ScopeDisplay *display, ScopeCapture *capture)
{
    if (atomic_load(&capture->ready) == 0)
    {
        return;
    }

    float captured_samples[SCOPE_CAPTURE_SAMPLES];

    for (int n = 0; n < SCOPE_CAPTURE_SAMPLES; n++)
    {
        captured_samples[n] = capture->samples[n];
    }

    capture->count = 0;
    atomic_store(&capture->ready, 0);

    int start = 0;

    for (int n = 1; n <= SCOPE_SAMPLES; n++)
    {
        if (captured_samples[n - 1] <= 0.0f && captured_samples[n] > 0.0f)
        {
            start = n;
            break;
        }
    }

    for (int n = 0; n < SCOPE_SAMPLES; n++)
    {
        display->samples[n] = captured_samples[start + n];
    }
}

void scope_display_draw(const ScopeDisplay *display)
{
    const int left = 70;
    const int top = 40;
    const int width = 700;
    const int height = 300;
    const float amplitude_range = 0.25f;

    /* Horizontal grid: amplitude from +0.25 to -0.25. */
    for (int row = 0; row <= 10; row++)
    {
        int y = top + row * height / 10;
        float amplitude = amplitude_range - row * 0.05f;

        DrawLine(left, y, left + width, y, row == 5 ? GRAY : (Color){35, 35, 35, 255});

        if (row % 2 == 0 || row == 5)
        {
            DrawText(TextFormat("%+.2f", amplitude), 10, y - 5, 10, LIGHTGRAY);
        }
    }

    /* Vertical grid: one division every 2 ms. */
    for (int column = 0; column <= 10; column++)
    {
        int x = left + column * width / 10;

        DrawLine(x, top, x, top + height, (Color){35, 35, 35, 255});

        DrawText(TextFormat("%d", column * 2), x - 5, top + height + 10, 10, LIGHTGRAY);
    }

    DrawText("Amplitude", 10, 15, 14, LIGHTGRAY);
    DrawText("Time from trigger (ms)", 300, 375, 14, LIGHTGRAY);

    /* Draw the captured output samples. */
    for (int n = 0; n < SCOPE_SAMPLES - 1; n++)
    {
        float x1 = left + n * (float)width / SCOPE_SAMPLES;
        float x2 = left + (n + 1) * (float)width / SCOPE_SAMPLES;

        float center_y = top + height / 2.0f;
        float scale_y = height / (2.0f * amplitude_range);

        float y1 = center_y - display->samples[n] * scale_y;
        float y2 = center_y - display->samples[n + 1] * scale_y;

        DrawLineV((Vector2){x1, y1}, (Vector2){x2, y2}, GREEN);
    }
}
