import { lazy, Suspense, useMemo, useState } from "react"
import { Route, Routes, useLocation } from "react-router-dom"
import { ToastContainer } from "react-toastify"
import "react-toastify/dist/ReactToastify.css"
import { AnimatePresence, motion, useReducedMotion } from "framer-motion"

import { useThemeMode } from "./theme/useThemeMode"

import Navbar from "./components/Navbar"
import Footer from "./components/Footer"
import ProtectedRoute from "./components/ProtectedRoutes"
import AdminRouteLayout from "./components/AdminRouteLayout"

const Home = lazy(() => import("./routes/Home"))
const Authors = lazy(() => import("./routes/Authors"))
const Tours = lazy(() => import("./routes/Program").then((m) => ({ default: m.Tours })))
const Contact = lazy(() => import("./routes/Contact"))
const Admin = lazy(() => import("./routes/Admin"))
const Login = lazy(() => import("./routes/Login"))
const IcnariInNews = lazy(() => import("./routes/IcnariInNews"))

const Aboutnit = lazy(() => import("./components/aboutcomp/Aboutnit"))
const OrganisingCom = lazy(() => import("./components/aboutcomp/OrganisingCom"))
const Venue = lazy(() => import("./components/aboutcomp/Venue"))
const Abounithistory = lazy(() => import("./components/aboutcomp/Abounithistory"))
const Aboutconf = lazy(() => import("./components/aboutcomp/Aboutconf"))
const Accomodations = lazy(() => import("./components/aboutcomp/Accomodations"))
const SponsorshipPage = lazy(() => import("./components/aboutcomp/SponsorshipPage"))
const TechnicalProgrammeCommittee = lazy(() => import("./components/aboutcomp/TechnicalProgrammeCommittee"))
const IndustryProgrammeCommittee = lazy(() => import("./components/aboutcomp/IndustryProgrammeCommittee"))
const InternationalAdvisoryCommittee = lazy(() => import("./components/aboutcomp/InternationalAdvisoryCommittee"))

const Guidelines = lazy(() => import("./components/authorcomp/Guidelines"))
const Papersub = lazy(() => import("./components/authorcomp/Papersub"))
const Registrations = lazy(() => import("./components/authorcomp/Registrations"))
const BestStuden = lazy(() => import("./components/authorcomp/BestStuden"))
const FinancialSupp = lazy(() => import("./components/authorcomp/FinancialSupp"))
const CMTAcknowledgement = lazy(() => import("./components/authorcomp/CMTAcknowledgement"))
const CallforPapers = lazy(() => import("./components/authorcomp/CallforPapers"))
const Track = lazy(() => import("./components/authorcomp/Track"))
const Publication = lazy(() => import("./components/authorcomp/Publication"))

const AllSpeakerprog = lazy(() => import("./components/programscomp/AllSpeakersprog"))
const TechnicalSession = lazy(() => import("./components/programscomp/TechnicalSession"))
const Culturalevents = lazy(() => import("./components/programscomp/Culturalevents"))

const BenefitsOfBecomeSponser = lazy(() =>
  import("./routes/Sponsors").then((m) => ({ default: m.BenefitsOfBecomeSponser }))
)

const AllPapersUser = lazy(() => import("./components/AllPapersUser"))
const AllUpdatesUser = lazy(() => import("./components/AllUpdatesUser"))
const AllImages = lazy(() => import("./components/AllImages"))

const AddSpeakers = lazy(() => import("./components/AddSpeakers"))
const AddRecentUpdates = lazy(() => import("./components/AddRecentUpdates"))
const AdminProfile = lazy(() => import("./components/AddAdmin"))
const AllSpeakers = lazy(() => import("./components/AllSpeakers"))
const AllUpdates = lazy(() => import("./components/AllUpdates"))
const AddPhotoGallery = lazy(() => import("./components/AddPhotoGallery"))
const AllMessages = lazy(() => import("./components/GetAllContact"))
const Allphotosgallery = lazy(() => import("./components/Allphotosgallery"))

const AddOrganisingCommitteeMember = lazy(() => import("./components/OrganisingCommittee/AddOrganisingCommitteeMember"))
const AllOrganisingCommitteeMembers = lazy(() => import("./components/OrganisingCommittee/AllOrganisingCommitteeMembers"))
const UpdateMember = lazy(() => import("./components/OrganisingCommittee/UpdateMember"))
const UpdateSpeaker = lazy(() => import("./components/UpdateSpeaker"))

const InternationalMember = lazy(() => import("./components/InternationalCommittee/AddMember"))
const AllInternationalAdvisoryCommitteeMembers = lazy(() => import("./components/InternationalCommittee/AllMembers"))
const UpdateInternationalMember = lazy(() => import("./components/InternationalCommittee/UpdateMember"))

const AddTechnicalCommitteeMember = lazy(() => import("./components/TechnicalCommittee/AddMember"))
const UpdateTechnicalMember = lazy(() => import("./components/TechnicalCommittee/UpdateMember"))
const AllTechnicalCommitteeMembers = lazy(() => import("./components/TechnicalCommittee/AllMembers"))

const AddIndustryProgrammeCommitteeMember = lazy(() => import("./components/IndustryProgrammeCommittee/AddMember"))
const UpdateIndustryProgrammeMember = lazy(() => import("./components/IndustryProgrammeCommittee/UpdateMember"))
const AllIndustryProgrammeCommitteeMembers = lazy(() => import("./components/IndustryProgrammeCommittee/AllMembers"))

const AddProgrammeCommitteeMember = lazy(() => import("./components/ProgrammeCommittee/AddMember"))
const UpdateProgrammeMember = lazy(() => import("./components/ProgrammeCommittee/UpdateMember"))
const AllProgrammeCommitteeMembers = lazy(() => import("./components/ProgrammeCommittee/AllMembers"))

function RouteLoader() {
  return (
    <div className="site-container py-16 text-center text-zinc-500">
      Loading…
    </div>
  )
}

function PageTransition({ children, routeKey }) {
  const shouldReduceMotion = useReducedMotion()
  const MotionDiv = motion.div

  const variants = useMemo(() => {
    if (shouldReduceMotion) {
      return {
        initial: { opacity: 1 },
        animate: { opacity: 1 },
        exit: { opacity: 1 },
      }
    }

    return {
      initial: { opacity: 0, y: 10 },
      animate: {
        opacity: 1,
        y: 0,
        transition: {
          duration: 0.45,
          ease: [0.16, 1, 0.3, 1],
        },
      },
      exit: {
        opacity: 0,
        y: -6,
        transition: {
          duration: 0.25,
          ease: [0.16, 1, 0.3, 1],
        },
      },
    }
  }, [shouldReduceMotion])

  return (
    <MotionDiv
      key={routeKey}
      variants={variants}
      initial="initial"
      animate="animate"
      exit="exit"
      style={{ willChange: shouldReduceMotion ? undefined : "transform, opacity" }}
    >
      {children}
    </MotionDiv>
  )
}

function App() {
  const [fetch,setfetch]=useState(false)
  const themeMode = useThemeMode()
  const location = useLocation();
  const isAdminRoute = location.pathname.startsWith("/admin");
  const isLoginRoute = location.pathname === "/login";
  const showPublicChrome = !(isAdminRoute || isLoginRoute);
  return (
    <div className="flex flex-col min-h-screen dark:bg-slate-900">
      {showPublicChrome ? <Navbar fetch={fetch} setfetch={setfetch}/> : null}
      <main className={showPublicChrome ? "flex-grow mt-6" : "flex-grow"}>
      <ToastContainer theme={themeMode} />
      <Suspense fallback={<RouteLoader />}>
        <AnimatePresence mode="wait" initial={false}>
        <PageTransition routeKey={location.pathname}>
        <Routes location={location}>
          <Route path="/" element={<Home />} />
          {/* About routes */}
          <Route path="/about" element={<Aboutnit />} />
          <Route path="/about/about-the-conference" element={<Aboutconf/>} />
          <Route path="/about/about-nit-patna" element={<Aboutnit />} />
          <Route path="/about/organising-committee" element={<OrganisingCom />} />
          <Route path="/about/venue-and-travels" element={<Venue />} />
          <Route path="/about/accomodations" element={<Accomodations />} />
          <Route path="/about/about-nit-patna-(bihta-campus)" element={<Abounithistory/>} />
          <Route path="/about/photogallery" element={<AllImages />} />
          <Route path="/about/technical-programme-committee" element={<TechnicalProgrammeCommittee />} />
          <Route path="/about/industry-programme-committee" element={<IndustryProgrammeCommittee />} />
          <Route path="/about/international-advisory-committee" element={<InternationalAdvisoryCommittee />} />

          {/* Authors routes */}
          <Route path="/authors" element={<Authors />} />
          <Route path="/authors/call-for-papers" element={<CallforPapers />} />
          <Route path="/authors/paper-publication" element={<Publication />} />
          <Route path="/authors/call-for-papers/:track" element={<Track />} />
          <Route path="/authors/guidelines-to-authors" element={<Guidelines />} />
          <Route path="/authors/paper-submissions" element={<Papersub/>} />
          <Route path="/authors/registrations" element={<Registrations/>} />
          <Route path="/authors/best-student-paper-award" element={<BestStuden/>} />
          <Route path="/authors/financial-support" element={<FinancialSupp/>} />
          <Route path="/authors/cmt-acknowledgement" element={<CMTAcknowledgement/>} />

          {/* Program routes */}
          <Route path="/programs/speakers" element={<AllSpeakerprog/>} />
          <Route path="/programs/tours" element={<Tours />} />
          <Route path="/programs/technical-session/:ts" element={<TechnicalSession />} />
          <Route path="/programs/cultural-event" element={<Culturalevents />} />

          {/* Sponser routes */}
          <Route path="/sponsors" element={<SponsorshipPage/>}></Route>
          <Route path="/sponsors/become-a-sponsor" element={<SponsorshipPage />} />
          <Route path="/sponsors/benefits-of-sponsorship" element={<BenefitsOfBecomeSponser />} />
          <Route path="/contact" element={<Contact />} />
          <Route path="/icnari-in-news" element={<IcnariInNews />} />
          <Route path="/allpapers" element={<AllPapersUser />} />
          <Route path="/allupdates" element={<AllUpdatesUser />} />
          <Route path="/login" element={<Login setfetch={setfetch}/>} />
          <Route element={<ProtectedRoute />}>
          {/* Admin routes */}
            <Route path="/admin" element={<AdminRouteLayout setfetch={setfetch} />}>
              <Route index element={<Admin />} />
              <Route path="add-speakers" element={<AddSpeakers />} />
              <Route path="add-recent-updates" element={<AddRecentUpdates />} />
              <Route path="profile" element={<AdminProfile />} />
              <Route path="all-speakers" element={<AllSpeakers />} />
              <Route path="all-speakers/update/:id" element={<UpdateSpeaker />} />
              <Route path="all-updates" element={<AllUpdates />} />
              <Route path="photogalleryupload" element={<AddPhotoGallery />} />
              <Route path="contact-messages" element={<AllMessages />} />
              <Route path="deletephoto" element={<Allphotosgallery />} />
              <Route path="add-organising-member" element={<AddOrganisingCommitteeMember />} />
              <Route path="all-organising-members" element={<AllOrganisingCommitteeMembers />} />
              <Route path="all-organising-members/:id" element={<UpdateMember />} />
              <Route path="add-international-member" element={<InternationalMember />} />
              <Route path="all-international-members" element={<AllInternationalAdvisoryCommitteeMembers />} />
              <Route path="all-international-members/:id" element={<UpdateInternationalMember />} />
              <Route path="add-technical-member" element={<AddTechnicalCommitteeMember />} />
              <Route path="all-technical-members" element={<AllTechnicalCommitteeMembers />} />
              <Route path="all-technical-members/:id" element={<UpdateTechnicalMember />} />
              <Route path="add-programme-member" element={<AddProgrammeCommitteeMember />} />
              <Route path="all-programme-members" element={<AllProgrammeCommitteeMembers />} />
              <Route path="all-programme-members/:id" element={<UpdateProgrammeMember />} />
              <Route path="add-industry-member" element={<AddIndustryProgrammeCommitteeMember />} />
              <Route path="all-industry-members" element={<AllIndustryProgrammeCommitteeMembers />} />
              <Route path="all-industry-members/:id" element={<UpdateIndustryProgrammeMember />} />
            </Route>
          </Route>
        </Routes>
        </PageTransition>
        </AnimatePresence>
      </Suspense>
      </main>
      {showPublicChrome ? <Footer/> : null}
    </div>
  )
}

export default App
