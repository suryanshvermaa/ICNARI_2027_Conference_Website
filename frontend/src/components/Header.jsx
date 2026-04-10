import { motion, useReducedMotion } from "framer-motion";

const Header = () => {
  const shouldReduceMotion = useReducedMotion();
  const MotionDiv = motion.div;
  const MotionImg = motion.img;

  const transition = shouldReduceMotion
    ? { duration: 0 }
    : { duration: 0.65, ease: [0.16, 1, 0.3, 1] };

  const containerVariants = shouldReduceMotion
    ? {}
    : {
        hidden: { opacity: 0 },
        show: {
          opacity: 1,
          transition: { staggerChildren: 0.07, delayChildren: 0.06 },
        },
      };

  const itemVariants = shouldReduceMotion
    ? {}
    : {
        hidden: { opacity: 0, y: 14 },
        show: { opacity: 1, y: 0, transition },
      };

  return (
    <header className="relative overflow-hidden bg-gradient-to-r from-gray-900 via-gray-700 to-gray-800 text-white py-16 sm:py-20 px-6 mt-6">
      {/* Overlay with gradient */}
      <div className="absolute inset-0 bg-black opacity-40 z-0 "></div>

      <MotionDiv
        className="container mx-auto text-center relative z-10 "
        variants={containerVariants}
        initial={shouldReduceMotion ? false : "hidden"}
        animate={shouldReduceMotion ? false : "show"}
      >
        <MotionDiv className="flex justify-center items-center space-x-6 mb-4" variants={itemVariants}>
          {/* Logo */}
          <MotionImg
            src="/vite.svg"
            alt="Conference Logo"
            className="rounded-full w-20 h-20 lg:w-28 lg:h-28 object-cover border-4 border-white"
            decoding="async"
            fetchPriority="high"
            variants={itemVariants}
          />
          {/* Conference Name */}
          <MotionDiv
            className="text-xl sm:text-2xl md:text-3xl font-extrabold text-white flex items-center"
            variants={itemVariants}
          >
            <h1>International Conference on Next-Generation Adaptive Research and Innovations 2027</h1>
          </MotionDiv>
        </MotionDiv>

        {/* Event Date */}
        <MotionDiv className="text-lg sm:text-xl font-semibold mb-6" variants={itemVariants}>
          <p className="bg-white/95 inline-block py-2 px-4 rounded-md text-gray-900 m-1.5 font-bold dark:bg-slate-800/80 dark:text-slate-50">
            Event Date: 6<sup>th</sup>, 7<sup>th</sup> & 8<sup>th</sup> of March, 2027
          </p>
        </MotionDiv>

        {/* Subheading / Call to Action */}
        <MotionDiv className="hidden md:block text-lg text-gray-200 font-bold" variants={itemVariants}>
          <p>Join the most innovative minds for a transformative event that pushes the boundaries of research and innovation.</p>
        </MotionDiv>
      </MotionDiv>
    </header>
  );
};

export default Header;
