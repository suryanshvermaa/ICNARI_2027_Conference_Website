import { useState, useEffect } from "react";
import { Link } from "react-router-dom";
import { AnimatePresence, motion, useReducedMotion } from "framer-motion";
import {
  HiMenuAlt3,
  HiX,
  HiChevronDown,
  HiMoon,
  HiSun,
} from "react-icons/hi";

import { applyTheme, getInitialTheme, setThemePreference } from "../theme/theme";

const technicalSessions=[
  'EVs and Emerging Technologies',
  'Photovoltaic Cell',
  'Renewable Energy',
  'Next Generation Sensing and Computing'
];

const menuSections = [
  { label: "About", key: "about", options: ["About the Conference", "About NIT Patna", "Organising Committee", "International Advisory Committee", "Technical Programme Committee", "Industry Programme Committee", "Venue and Travels", "Accomodations", "About NIT Patna (Bihta Campus)", "PhotoGallery"] },
  { label: "Authors", key: "authors", options: ["Call for Papers", "Guidelines to Authors", "Paper Submissions", "Registrations", "Best Student Paper Award", "Financial support", "CMT Acknowledgement", "Paper Publication"] },
  { label: "Programs", key: "programs", options: ["Speakers", "Technical Session", "Tours", "Cultural Event"] },
  { label: "Sponsors", key: "sponsors", options: ["Become a Sponsor", "Benefits of Sponsorship"] },
  // { label: "Contact", key: "contact", options: ["Contact Form"] },
];

export default function Navbar({ fetch, setfetch }) {
  const [isOpen, setIsOpen] = useState(false);
  const [isAuthenticated, setIsAuthenticated] = useState(false);
  const [openDropdown, setOpenDropdown] = useState("");
  const [openNestedDropdown, setOpenNestedDropdown] = useState("");
  const [activeDesktopDropdown, setActiveDesktopDropdown] = useState("");
  const [activeDesktopNested, setActiveDesktopNested] = useState("");
  const [theme, setTheme] = useState("light");

  const shouldReduceMotion = useReducedMotion();
  const MotionDiv = motion.div;
  const MotionSpan = motion.span;

  const dropdownTransition = shouldReduceMotion
    ? { duration: 0 }
    : { duration: 0.28, ease: [0.16, 1, 0.3, 1] };

  const dropdownVariants = shouldReduceMotion
    ? {
        initial: { opacity: 1 },
        animate: { opacity: 1 },
        exit: { opacity: 1 },
      }
    : {
        initial: { opacity: 0, y: 10, scale: 0.98 },
        animate: { opacity: 1, y: 0, scale: 1, transition: dropdownTransition },
        exit: { opacity: 0, y: 6, scale: 0.98, transition: dropdownTransition },
      };

  const toggleDropdown = (section) => {
    setOpenDropdown((prev) => (prev === section ? "" : section));
  };

  const toggleNestedDropdown = (section) => {
    setOpenNestedDropdown((prev) => (prev === section ? "" : section));
  };

  useEffect(() => {
    const token = localStorage.getItem("token");
    if (token) {
      setIsAuthenticated(true);
    }
  }, [setfetch, fetch]);

  useEffect(() => {
    setTheme(getInitialTheme());
  }, []);

  const toggleTheme = () => {
    const next = theme === "dark" ? "light" : "dark";
    setTheme(next);
    applyTheme(next);
    setThemePreference(next);
  };

  useEffect(() => {
    if (!isOpen) {
      setOpenDropdown("");
      setOpenNestedDropdown("");
    }
  }, [isOpen]);

  return (
    <nav className="fixed top-0 left-0 w-full z-50 border-b border-zinc-200 bg-white/90 text-zinc-900 shadow-sm backdrop-blur dark:border-slate-700/60 dark:bg-slate-900/80 dark:text-slate-50">
      <div className="mx-auto flex max-w-7xl items-center justify-between gap-3 px-3 py-2 sm:px-6">
        <Link to="/" className="flex items-center gap-2 font-bold">
          <img src="/vite.svg" className="h-14 w-16 sm:h-16 sm:w-20" alt="logo.svg" loading="lazy" decoding="async" />
        </Link>

        {/* Desktop Menu */}
        <div className="hidden md:flex items-center gap-10 px-4 text-sm font-semibold lg:text-base">
          <Link to="/" className="transition-colors hover:text-zinc-600 dark:hover:text-slate-200">Home</Link>

          {menuSections.map(({ label, key, options }) => (
            <div
              key={key}
              className="relative"
              onMouseEnter={() => setActiveDesktopDropdown(key)}
              onMouseLeave={() => {
                setActiveDesktopDropdown("");
                setActiveDesktopNested("");
              }}
            >
              <button
                type="button"
                className="inline-flex items-center gap-1 transition-colors hover:text-zinc-600 dark:hover:text-slate-200"
                aria-haspopup="menu"
                aria-expanded={activeDesktopDropdown === key}
              >
                {label}
                <MotionSpan
                  aria-hidden="true"
                  animate={
                    shouldReduceMotion
                      ? undefined
                      : { rotate: activeDesktopDropdown === key ? 180 : 0 }
                  }
                  transition={dropdownTransition}
                  className="inline-flex"
                >
                  <HiChevronDown />
                </MotionSpan>
              </button>

              <AnimatePresence>
                {activeDesktopDropdown === key ? (
                  <MotionDiv
                    variants={dropdownVariants}
                    initial="initial"
                    animate="animate"
                    exit="exit"
                    className="absolute top-full left-0 z-50 mt-2 min-w-[260px] overflow-visible rounded-xl border border-zinc-200 bg-white py-2 shadow-xl dark:border-slate-700 dark:bg-slate-900"
                  >
                    {options.map((opt, i) => {
                      if (opt === "Technical Session") {
                        return (
                          <div
                            key={i}
                            className="relative"
                            onMouseEnter={() => setActiveDesktopNested("technical-session")}
                            onMouseLeave={() => setActiveDesktopNested("")}
                          >
                            <div className="flex h-full w-full cursor-pointer items-center justify-between gap-2 px-4 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60">
                              <span>{opt}</span>
                              <span className="text-zinc-400 dark:text-slate-400">→</span>
                            </div>
                            <AnimatePresence>
                              {activeDesktopNested === "technical-session" ? (
                                <MotionDiv
                                  variants={dropdownVariants}
                                  initial="initial"
                                  animate="animate"
                                  exit="exit"
                                  className="absolute left-full top-0 z-50 min-w-[260px] overflow-hidden rounded-xl border border-zinc-200 bg-white py-2 shadow-xl dark:border-slate-700 dark:bg-slate-900"
                                >
                                  {technicalSessions.map((ses, j) => (
                                    <Link
                                      key={j}
                                      to={`/${key}/${opt
                                        .toLowerCase()
                                        .replace(/\s+/g, "-")}/${ses
                                        .toLowerCase()
                                        .replace(/\s+/g, "-")}`}
                                      className="block px-4 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60"
                                    >
                                      {ses}
                                    </Link>
                                  ))}
                                </MotionDiv>
                              ) : null}
                            </AnimatePresence>
                          </div>
                        );
                      }

                      if (opt === "CMT Acknowledgement") {
                        return (
                          <a
                            key={i}
                            href={`/${key}/${opt
                              .toLowerCase()
                              .replace(/\s+/g, "-")}.html`}
                            className="block px-4 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60"
                          >
                            {opt}
                          </a>
                        );
                      }

                      return (
                        <Link
                          key={i}
                          to={`/${key}/${opt.toLowerCase().replace(/\s+/g, "-")}`}
                          className="block px-4 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60"
                        >
                          {opt}
                        </Link>
                      );
                    })}
                  </MotionDiv>
                ) : null}
              </AnimatePresence>
            </div>
          ))}
          <Link to="/contact" className="transition-colors hover:text-zinc-600 dark:hover:text-slate-200">Contact</Link>
          <Link to="/icnari-in-news" className="transition-colors hover:text-zinc-600 dark:hover:text-slate-200">ICNARI in news</Link>
          {isAuthenticated && (
            <Link to="/admin" className="font-semibold transition-colors hover:text-zinc-600 dark:hover:text-slate-200">Admin</Link>
          )}
        </div>

        {/* Desktop Theme Toggle */}
        <button
          type="button"
          onClick={toggleTheme}
          className="hidden md:inline-flex items-center justify-center rounded-lg border border-zinc-200 bg-white/70 px-2 py-2 transition-colors hover:bg-zinc-100 dark:border-slate-700/60 dark:bg-slate-900/40 dark:hover:bg-slate-800"
          aria-label={theme === "dark" ? "Switch to light mode" : "Switch to dark mode"}
        >
          {theme === "dark" ? <HiSun size={20} /> : <HiMoon size={20} />}
        </button>

        {/* Mobile Controls */}
        <div className="md:hidden flex items-center gap-2">
          <button
            type="button"
            onClick={toggleTheme}
            className="rounded-lg border border-zinc-200 bg-white/70 px-2 py-2 transition-colors hover:bg-zinc-100 dark:border-slate-700/60 dark:bg-slate-900/40 dark:hover:bg-slate-800"
            aria-label={theme === "dark" ? "Switch to light mode" : "Switch to dark mode"}
          >
            {theme === "dark" ? <HiSun size={20} /> : <HiMoon size={20} />}
          </button>

          <button
            className="rounded-lg px-2 py-2 transition-colors hover:bg-zinc-100 dark:hover:bg-slate-800"
            onClick={() => setIsOpen(!isOpen)}
            aria-label={isOpen ? "Close menu" : "Open menu"}
          >
            {isOpen ? <HiX size={28} /> : <HiMenuAlt3 size={28} />}
          </button>
        </div>
      </div>

      {/* Mobile Menu */}
      <AnimatePresence>
        {isOpen ? (
          <MotionDiv
            initial={shouldReduceMotion ? { opacity: 1 } : { opacity: 0, height: 0 }}
            animate={shouldReduceMotion ? { opacity: 1 } : { opacity: 1, height: "auto" }}
            exit={shouldReduceMotion ? { opacity: 1 } : { opacity: 0, height: 0 }}
            transition={dropdownTransition}
            className="md:hidden flex flex-col justify-between bg-white/95 p-4 space-y-2 z-40 shadow-lg border-t border-zinc-200 overflow-hidden dark:border-slate-700/60 dark:bg-slate-900/95"
          >
            <Link to="/" className="rounded-md px-2 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60" onClick={() => setIsOpen(false)}>Home</Link>

            {menuSections.map(({ label, key, options }) => (
              <div key={key}>
                <button
                  type="button"
                  className="flex items-center justify-between w-full cursor-pointer rounded-md px-2 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60"
                  onClick={() => toggleDropdown(key)}
                  aria-expanded={openDropdown === key}
                >
                  <span>{label}</span>
                  <MotionSpan
                    aria-hidden="true"
                    animate={
                      shouldReduceMotion
                        ? undefined
                        : { rotate: openDropdown === key ? 180 : 0 }
                    }
                    transition={dropdownTransition}
                    className="inline-flex"
                  >
                    <HiChevronDown />
                  </MotionSpan>
                </button>

                <AnimatePresence>
                  {openDropdown === key ? (
                    <MotionDiv
                      initial={shouldReduceMotion ? { opacity: 1 } : { opacity: 0, height: 0 }}
                      animate={shouldReduceMotion ? { opacity: 1 } : { opacity: 1, height: "auto" }}
                      exit={shouldReduceMotion ? { opacity: 1 } : { opacity: 0, height: 0 }}
                      transition={dropdownTransition}
                      className="ml-4 mt-1 flex flex-col space-y-1 overflow-hidden"
                    >
                      {options.map((opt, i) => {
                        if (opt === "Technical Session") {
                          return (
                            <div key={i}>
                              <button
                                type="button"
                                className="flex items-center justify-between w-full cursor-pointer rounded-md px-2 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60"
                                onClick={() => toggleNestedDropdown("technical-session")}
                                aria-expanded={openNestedDropdown === "technical-session"}
                              >
                                <span>{opt}</span>
                                <MotionSpan
                                  aria-hidden="true"
                                  animate={
                                    shouldReduceMotion
                                      ? undefined
                                      : {
                                          rotate:
                                            openNestedDropdown === "technical-session"
                                              ? 180
                                              : 0,
                                        }
                                  }
                                  transition={dropdownTransition}
                                  className="inline-flex"
                                >
                                  <HiChevronDown />
                                </MotionSpan>
                              </button>
                              <AnimatePresence>
                                {openNestedDropdown === "technical-session" ? (
                                  <MotionDiv
                                    initial={shouldReduceMotion ? { opacity: 1 } : { opacity: 0, height: 0 }}
                                    animate={shouldReduceMotion ? { opacity: 1 } : { opacity: 1, height: "auto" }}
                                    exit={shouldReduceMotion ? { opacity: 1 } : { opacity: 0, height: 0 }}
                                    transition={dropdownTransition}
                                    className="ml-4 mt-1 flex flex-col space-y-1 max-h-64 overflow-y-auto"
                                  >
                                    {technicalSessions.map((ses, j) => (
                                      <Link
                                        key={j}
                                        to={`/${key}/${opt
                                          .toLowerCase()
                                          .replace(/\s+/g, "-")}/${ses
                                          .toLowerCase()
                                          .replace(/\s+/g, "-")}`}
                                        className="rounded-md px-2 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60 whitespace-normal break-words"
                                        onClick={() => setIsOpen(false)}
                                      >
                                        {ses}
                                      </Link>
                                    ))}
                                  </MotionDiv>
                                ) : null}
                              </AnimatePresence>
                            </div>
                          );
                        }

                        if (opt === "CMT Acknowledgement") {
                          return (
                            <a
                              key={i}
                              href={`/${key}/${opt
                                .toLowerCase()
                                .replace(/\s+/g, "-")}.html`}
                              className="rounded-md px-2 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60"
                              onClick={() => setIsOpen(false)}
                            >
                              {opt}
                            </a>
                          );
                        }

                        return (
                          <Link
                            key={i}
                            to={`/${key}/${opt.toLowerCase().replace(/\s+/g, "-")}`}
                            className="rounded-md px-2 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60"
                            onClick={() => setIsOpen(false)}
                          >
                            {opt}
                          </Link>
                        );
                      })}
                    </MotionDiv>
                  ) : null}
                </AnimatePresence>
              </div>
            ))}

            <Link to="/contact" className="rounded-md px-2 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60" onClick={() => setIsOpen(false)}>Contact</Link>
            <Link
              to="/icnari-in-news"
              className="rounded-md px-2 py-2 transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60"
              onClick={() => setIsOpen(false)}
            >
              ICNARI in news
            </Link>
            {isAuthenticated && (
              <Link
                to="/admin"
                className="rounded-md px-2 py-2 font-semibold transition-colors hover:bg-zinc-50 dark:hover:bg-slate-800/60"
                onClick={() => setIsOpen(false)}
              >
                Admin
              </Link>
            )}
          </MotionDiv>
        ) : null}
      </AnimatePresence>
    </nav>
  );
}
