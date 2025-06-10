import { $, component$, useSignal } from "@builder.io/qwik";
import { type DocumentHead } from "@builder.io/qwik-city";
import { HOST } from "../consts/host";

/**
 * %STATE%        is a placeholder and will be replaced serverside on the ESP32 (SSR)
 * Qwik useSignal https://qwik.dev/docs/getting-started/#4-modifying-state
 * fetch          https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API/Using_Fetch
 */

export default component$(() => {
  const state = useSignal<number>();

  const requestOn = $(async () => {
    const res = await fetch(`http://${HOST}/api/on`);
    const stateResponse = await res.json();
    state.value = stateResponse.state;
  });

  const requestOff = $(async () => {
    const res = await fetch(`http://${HOST}/api/off`);
    const stateResponse = await res.json();
    state.value = stateResponse.state;
  });

  const requestState = $(async () => {
    const res = await fetch(`http://${HOST}/api/state-changed`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        state: state.value,
      }),
    });
    const stateResponse = await res.json();
    state.value = stateResponse.state;
  });

  return (
    <>
      <h1>T-QT-C6 with Qwik frontend</h1>

      <div class="range-slider">
        <input
          class="range-slider__range"
          type="range"
          min="0"
          max="255"
          value={state.value ?? "%STATE%"}
          onChange$={async (_event: Event, element: HTMLInputElement) => {
            state.value = parseInt(element.value, 10);
            await requestState();
          }}
        />
        <span class="range-slider__value">{state.value ?? "%STATE%"}</span>
      </div>

      <button onClick$={requestOn} class="button">
        ON
      </button>
      <button onClick$={requestOff} class="button secondary">
        OFF
      </button>
    </>
  );
});

export const head: DocumentHead = {
  title: "T-QT-C6 Web Server",
  meta: [
    {
      name: "description",
      content: "UI example with WiFi Access Point",
    },
  ],
};
