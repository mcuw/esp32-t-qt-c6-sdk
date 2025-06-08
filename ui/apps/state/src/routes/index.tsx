import { $, component$, useSignal } from "@builder.io/qwik";
import type { DocumentHead } from "@builder.io/qwik-city";
import { HOST } from "../consts/host";

// export async function getState(controller?: AbortController): Promise<string> {
//   console.log("FETCH", `/api/state`);
//   const resp = await fetch(`/api/state`, {
//     signal: controller?.signal,
//   });
//   console.log("FETCH resolved");
//   const json = await resp.json();
//   return json.state;
// }

export default component$(() => {
  const state = useSignal("%STATE%");

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

  return (
    <>
      <h1>T-QT-C6</h1>
      <p>State: {state.value}</p>
      <p>
        <button
          onClick$={async () => {
            await requestOn();
          }}
          class="button"
        >
          ON
        </button>
      </p>
      <p>
        <button
          onClick$={async () => {
            await requestOff();
          }}
          class="button secondary"
        >
          OFF
        </button>
      </p>
    </>
  );
});

export const head: DocumentHead = {
  title: "T-QT-C6 Web Server",
  meta: [
    {
      name: "description",
      content: "UI example with Wifi AP",
    },
  ],
};
