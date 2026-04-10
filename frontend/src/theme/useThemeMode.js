import { useEffect, useState } from "react";
import { THEME_CHANGE_EVENT, getInitialTheme } from "./theme";

export function useThemeMode() {
  const [theme, setTheme] = useState(() => getInitialTheme());

  useEffect(() => {
    const update = () => setTheme(getInitialTheme());

    window.addEventListener(THEME_CHANGE_EVENT, update);
    window.addEventListener("storage", update);

    const media = window.matchMedia?.("(prefers-color-scheme: dark)");
    media?.addEventListener?.("change", update);

    return () => {
      window.removeEventListener(THEME_CHANGE_EVENT, update);
      window.removeEventListener("storage", update);
      media?.removeEventListener?.("change", update);
    };
  }, []);

  return theme;
}
